#!/usr/bin/env python

import unittest
import numpy as np
import configparser
import os


class UniformModel(object):

    def __init__(self):
        self.model = {"model":"model","model_name":"uniform"}


#------------------------------------------------------------------------------
    def add_fields(self,B=(1.,0.,0.),E=(0.,0.,0.)):
        """
        defines electromagnetic fields of a uniform model_name
        default is B=1.e_x and E=0.
        """
        if len(B) != 3 or not isinstance(B,tuple) or not isinstance(B,list):
            ValueError("invalid B")
        if len(E) != 3 or not isinstance(E,tuple) or not isinstance(E,list):
            ValueError("invalid E")

        self.model.update({"bx":B[0],
                    "by":B[1],
                    "bz":B[2],
                    "ex":E[0],
                    "ey":E[1],
                    "ez":E[2]})

#------------------------------------------------------------------------------

    def nbr_species(self):
        """
        returns the number of species currently registered in the model
        """
        keys = self.model.keys()
        nbr = 0
        for k in keys:
            if k.startswith('species'):
                nbr += 1
        return nbr

#------------------------------------------------------------------------------

    def add_species(self,name,
                    charge=1,
                    mass=1,
                    nbrPartCell=100,
                    density=1.,
                    vbulk=(0.,0.,0.),
                    beta=1.0,
                    anisotropy=1.):
        """
        add a species to the current model

        add_species(name,charge=1, mass=1, nbrPartCell=100, density=1, vbulk=(0,0,0), beta=1, anisotropy=1)

        Parameters:
        -----------
        name        : name of the species, str

        Optional Parameters:
        -------------------
        charge      : charge of the species particles, float (default = 1.)
        nbrPartCell : number of particles per cell, int (default = 100)
        density     : particle density, float (default = 1.)
        vbulk       : bulk velocity, tuple of size 3  (default = (0,0,0))
        beta        : beta of the species, float (default = 1)
        anisotropy  : Pperp/Ppara of the species, float (default = 1)
        """

        idx=str(self.nbr_species())

        new_species = {"speciesName"+idx:name,
                       "charge"+idx:charge,
                       "mass"+idx:mass,
                       "density"+idx:density,
                       "vbulk"+idx:vbulk,
                       "beta"+idx:beta,
                       "anisotropy"+idx:anisotropy,
                       "nbrParticlesPerCell"+idx:nbrPartCell}

        keys = self.model.keys()
        if "speciesName"+idx in keys:
            raise ValueError("species already registered")

        self.model.update(new_species)
#------------------------------------------------------------------------------

    def to_dict(self):
        return self.model

#------------------------------------------------------------------------------

def add_dict_to_config(the_dict,conf, section_key="section"):
    """
    adds the dictionnary 'the_dict' to a config 'conf' under the section 'section_key'
    if the section does not exist, it is created
    """
    sections = conf.sections()
    this_section = the_dict[section_key]
    if this_section not in sections:
        conf.add_section(this_section)
    for key in the_dict.keys():
        if key != section_key:
            conf.set(the_dict[section_key], key, str(the_dict[key]))

#------------------------------------------------------------------------------


class Simulation(object):
    """represents a simulation input"""

    fluid_diag_types = ['rho_s', 'flux_s']
    em_diag_types=['E', 'B']


    def __init__(self, **kwargs):
        """
        1D run example: Simulation(time_step_nbr = 100, boundary_types="periodic", cells=80)
        2D run example: Simulation(time_step_nbr = 100, boundary_types=("periodic","periodic"), cells=(80,20))
        3D run example: Simulation(time_step_nbr = 100, boundary_types=("periodic","periodic","periodic"), cells=(80,20,40))

        optional parameters:
        -------------------

        dl               : grid spacing dx, (dx,dy) or (dx,dy,dz) in 1D, 2D or 3D
                           must be specified if 'domain_size' is not set
        domain_size      : size of the physical domain Lx, (Lx,Ly), (Lx,Ly,Lz) in 1D, 2D or 3D
                           must be specified if 'dl' is not set
        final_time       : final simulation time. Must be set if 'time_step' is not
        time_step        : simulation time step. Must be specified if 'final_time' is not
        interp_order     : order of the particle/mesh interpolation. Either 1, 2, 3 or 4 (default=1)
        layout           : layout of the physical quantities on the mesh (default = "yee")
        origin           : origin of the physical domain, (default (0,0,0) in 3D)
        splitting_method : method to split particles (default= "splitOrderN_RF2")
        particle_pusher  : algo to push particles (default = "modifiedBoris")

        """

        if 'time_step_nbr' not in kwargs:
            raise ValueError("Error : specify 'time_step_nbr'")

        if 'boundary_types' not in kwargs:
            raise ValueError("Error : specify 'boundary_types'")

        if 'cells' not in kwargs:
            raise ValueError("Error : specify 'cells'")

                 #layout="yee",
                 #nbr_ion_populations=1,
                 #particle_pusher="modifiedBoris",
                 #splitting_method="splitOrderN_RF2",
                 #origin=(0,0,0),
                 #path='.'):

        super(Simulation, self).__init__()

        if 'path' in kwargs:
            self.path = kwargs['path']
        else:
            self.path = './'

        def missingInfo(a,b):
            if a is None or b is None:
                raise ValueError("not enough information to build domain")
            if not (isinstance(a,list) or isinstance(a,tuple)) or not \
            (isinstance(b,tuple) or isinstance(b,list)):
                raise ValueError("not iterable")


        if 'domain_size' not in kwargs:
            missingInfo(kwargs['cells'], kwargs['dl'])
            self.domain_size = [cell*d for cell,d in zip(kwargs["cells"],kwargs['dl'])]
            self.cells = kwargs['cells']
            self.dl = kwargs['dl']

        elif 'dl' not in kwargs:
            missingInfo(kwargs['domain_size'],kwargs['cells'])
            self.dl= []
            for L, n in zip(kwargs['domain_size'], kwargs['cells']):
                if n == 0:
                    self.dl.append(-1)
                else:
                    self.dl.append(L/float(n))
            self.cells = kwargs['cells']
            self.domain_size = kwargs['domain_size']


        self.time_step_nbr = kwargs['time_step_nbr']
        if 'final_time' in kwargs and 'time_step' in kwargs:
            raise ValueError("specify either time_step or final_time not both")

        if 'final_time' in kwargs:
            self.time_step = kwargs['final_time']/float(kwargs['time_step_nbr'])
            self.final_time = kwargs['final_time']
        else:
            if 'time_step' in kwargs:
                self.final_time = kwargs['time_step_nbr'] * kwargs['time_step']
                self.time_step = kwargs['time_step']
            else:
                raise ValueError("specify time_step or final_time")

        # search dimension
        cells = kwargs['cells']
        if cells[2] == 0:
            if cells[1] == 0:
                self.dims = 1
            else:
                self.dims=2
        else:
            if cells[0] == 0:
                raise ValueError("Invalid 'cells'")
            else:
                self.dims=3

        if 'interp_order' not in kwargs:
            self.interp_order=1

        elif kwargs['interp_order'] not in [1,2,3,4]:
            raise ValueError("invalid interpolation order")
        else:
            self.interp_order = kwargs['interp_order']


        error_bc_dim = "boundary_types must have {} dimension".format(self.dims)
        if isinstance(kwargs['boundary_types'],list) or isinstance(kwargs['boundary_types'],tuple):
            if self.dims == len(kwargs['boundary_types']):
                self.boundary_types = kwargs['boundary_types']
            else:
                raise ValueError(error_bc_dim)
        else:
            if self.dims==1:
                self.boundary_types = []
                self.boundary_types.append(kwargs['boundary_types'])
            else:
                print(self.cells, kwargs['boundary_types'])
                raise ValueError(error_bc_dim)


        # TODO: nbr_ion_populations must be a function that counts the species in a model
        #self.nbr_ion_populations = nbr_ion_populations

        if 'origin' in kwargs:
            self.origin = kwargs['origin']
        else:
            self.origin = (0,0,0)

        if 'layout' in kwargs:
            self.layout = kwargs['layout']
        else:
            self.layout = "yee"

        if 'splitting_method' in kwargs:
            self.splitting_method = kwargs['splitting_method']
        else:
            self.splitting_method = "splitOrderN_RF2"

        if 'particle_pusher' not in kwargs:
            self.particle_pusher = "modifiedBoris"
        else:
            if particle_pusher not in ["modifiedBoris"]:
                raise ValueError("invalid pusher")
            self.particle_pusher = "modifiedBoris"


        self.fluid_diagnostics=[]
        self.em_diagnostics=[]
        self.particle_diagnostics=[]

#------------------------------------------------------------------------------
    def add_fluid_diagnostics(self, **kwargs):
        """
        Example :

        simu.add_fluid_diagnostics(name='FluidDiagnostics1',
                                   diag_type='rho_s',
                                   write_every=10,
                                   compute_every=5,
                                   start_iteration=0,
                                   last_iteration=1000,
                                   species_name='proton1')

        if 'compute_every' is not specified, then it is set to 'write_every'
        if 'last_iteration' is not specified then it is set to the simulation nbr of time steps
        if 'start_iteration' is not specified then it is 0
        """

        if 'write_every' not in kwargs:
            raise ValueError("Error: specify 'write_every'")

        if 'name' not in kwargs:
            raise ValueError("Error: specify 'name'")

        if 'diag_type' not in kwargs:
            raise ValueError("Error: specify 'diag_type'")

        if 'species_name' not in kwargs:
            raise ValueError("Error: specify 'species_name'")

        if 'start_iteration' not in kwargs:
            start_iteration = 0
        else:
            start_iteration = kwargs['start_iteration']

        if 'last_iteration' not in kwargs:
            last_iteration = self.time_step_nbr
        else:
            last_iteration = kwargs['last_iteration']

        if 'path' not in kwargs:
            path = kwargs['name'].lower()
        else:
            path = kwargs['path']

        if kwargs['diag_type'] not in Simulation.fluid_diag_types:
            raise ValueError("invalid diagnostic type : {}".format(kwargs['diag_type']))

        if 'compute_every' not in kwargs:
            compute_every = kwargs['write_every']
        else:
            compute_every = kwargs['compute_every']

        if kwargs['write_every'] < compute_every:
            raise ValueError("write_every should be >= compute_every")

        if start_iteration >= last_iteration:
            raise ValueError("iStart should be < iEnd")

        for diag in self.fluid_diagnostics:
            if kwargs['name'] == diag["name"]:
                raise ValueError("diagnostic name already used")


        self.fluid_diagnostics.append({"name":kwargs['name'],
                                    "diagCategory":"FluidDiagnostics",
                                    "diagType":kwargs['diag_type'],
                                    "writeEvery":kwargs['write_every'],
                                    "computeEvery":compute_every,
                                    "iStart":start_iteration,
                                    "iEnd":last_iteration,
                                    "speciesName":kwargs['species_name'],
                                    "path":path})
#------------------------------------------------------------------------------

    def add_electromag_diagnostics(self, **kwargs):

        """
        Example :

        simu.add_electromag_diagnostics(name='ElectromagDiagnostics1',
                                        diag_type='E',
                                        write_every=10,
                                        compute_every=5,
                                        start_iteration=0,
                                        last_iteration=1000)

        if 'compute_every' is not specified, then it is set to 'write_every'
        if 'last_iteration' is not specified then it is set to the simulation nbr of time steps
        if 'start_iteration' is not specified then it is 0
        """

        if 'write_every' not in kwargs:
            raise ValueError("Error: specify 'write_every'")

        if 'name' not in kwargs:
            raise ValueError("Error: specify 'name'")

        if 'diag_type' not in kwargs:
            raise ValueError("Error: specify 'diag_type'")


        if 'start_iteration' not in kwargs:
            start_iteration = 0
        else:
            start_iteration = kwargs['start_iteration']

        if 'last_iteration' not in kwargs:
            last_iteration = self.time_step_nbr
        else:
            last_iteration = kwargs['last_iteration']

        if 'path' not in kwargs:
            path = kwargs['name'].lower()
        else:
            path = kwargs['path']

        if kwargs['diag_type'] not in Simulation.em_diag_types:
            raise ValueError("invalid diagnostic type : {}".format(kwargs['diag_type']))

        if 'compute_every' not in kwargs:
            compute_every = kwargs['write_every']
        else:
            compute_every = kwargs['compute_every']

        if kwargs['write_every'] < compute_every:
            raise ValueError("write_every should be >= compute_every")

        if start_iteration >= last_iteration:
            raise ValueError("iStart should be < iEnd")

        for diag in self.fluid_diagnostics:
            if kwargs['name'] == diag["name"]:
                raise ValueError("diagnostic name already used")

        self.em_diagnostics.append({"name":kwargs['name'],
                                    "diagCategory":"ElectromagDiagnostics",
                                    "diagType":kwargs['diag_type'],
                                    "writeEvery":kwargs['write_every'],
                                    "computeEvery":compute_every,
                                    "iStart":start_iteration,
                                    "iEnd":last_iteration,
                                    "path":path})


#------------------------------------------------------------------------------


    def write_ini_file(self, filename="phare.ini"):

        config = configparser.ConfigParser()
        config.add_section('Simulation')

        dim_names=('x','y','z')
        for d in np.arange(self.dims):
            config.set('Simulation', 'numOfCell'+dim_names[d],str(self.cells[d]))
            config.set('Simulation', 'd'+dim_names[d],str(self.dl[d]))
            config.set('Simulation','origin'+dim_names[d], str(self.origin[d]))
            config.set('Simulation', 'boundaryCondition'+dim_names[d],
                            self.boundary_types[d])



        params = {"section":"Simulation",
                  "layout":str(self.layout),
                  "interpOrder":str(self.interp_order),
                  "timeStep":str(self.time_step),
                  "particlePusher":self.particle_pusher,
                  "splittingMethod":self.splitting_method,
                  "numOfSteps":str(self.time_step_nbr)}


        add_dict_to_config(params, config)

        for fluid_diag in self.fluid_diagnostics:
            add_dict_to_config(fluid_diag, config, section_key="name")

        for em_diag in self.em_diagnostics:
            add_dict_to_config(em_diag, config, section_key="name")

        add_dict_to_config(self.model.to_dict(), config, section_key="model")

        with open(os.path.join(self.path,filename),"w") as config_file:
            config.write(config_file)

#------------------------------------------------------------------------------

    def add_model(self,model):
        self.model = model

#------------------------------------------------------------------------------



###############################################################################
#                               UNIT TESTS                                    #
###############################################################################

class TestSimulation(unittest.TestCase):

    def setUp(self):
        self.cells_array = [(80,0,0),(80,40,0), (80,40,12)]
        self.dl_array = [(0.1,0,0), (0.1,0.2,0.), (0.1,0.2,0.3)]
        self.domain_size_array = [(100,0,0), (100,80,0.), (100,80,20)]
        self.dims = [1,2,3]
        self.bcs = ["periodic", ("periodic", "periodic"), ("periodic", "periodic", "periodic")]
        self.layout = "yee"
        self.time_step  = 0.001
        self.time_step_nbr  = 1000
        self.final_time = 1.

    def test_domain_size(self):
        for cells,dl,dim,bc in zip(self.cells_array, self.dl_array, self.dims,self.bcs):
            j = Simulation(time_step_nbr=self.time_step_nbr,
                           boundary_types=bc,
                           cells=cells ,dl=dl, final_time=self.final_time)

            for d in np.arange(dim):
                self.assertEqual(j.domain_size[d], j.dl[d]*cells[d])


    def test_dl(self):
        for cells,domain_size,dim, bc in zip(self.cells_array,
                                             self.domain_size_array,
                                             self.dims,
                                             self.bcs):
            j = Simulation(time_step_nbr=self.time_step_nbr,
                           boundary_types=bc, cells=cells ,
                           domain_size=domain_size, final_time=self.final_time)

            for d in np.arange(dim):
                self.assertEqual(j.dl[d], domain_size[d]/float(cells[d]))



    def test_boundary_conditions(self):
        j = Simulation(time_step_nbr=1000, boundary_types="periodic",
                       cells=(80,0,0), domain_size=(10,0,0), final_time=1.)

        for d in np.arange(j.dims):
            self.assertEqual("periodic", j.boundary_types[d])


    def test_assert_boundary_condition(self):
        j= Simulation(time_step_nbr=1000,
                      boundary_types="periodic",
                      cells=(80,0,0), domain_size=(10,0,0), final_time=1000)


    def test_final_time(self):
        s = Simulation(time_step_nbr=1000,
                       boundary_types="periodic",
                       cells=(80,0,0),domain_size=(10,0,0), time_step=0.01)

        self.assertEqual(10,s.final_time)

    def test_time_step(self):
        s = Simulation(time_step_nbr=1000,
                       boundary_types="periodic",
                       cells=(80,0,0), domain_size=(10,0,0), final_time=10)

        self.assertEqual(0.01,s.time_step)



    def test_ini_file(self):
        """ serialize a simulation in an INI file and read it to check it is OK"""

        simu = Simulation(time_step_nbr=1000,
                          boundary_types="periodic",
                          cells=(80,0,0),
                          dl=(0.1,0,0),
                          final_time=1.)

        simu.add_fluid_diagnostics(name='FluidDiagnostics1',
                                   diag_type='rho_s',
                                   write_every=10,
                                   compute_every=5,
                                   start_iteration=0,
                                   last_iteration=1000,
                                   species_name='proton1')

        simu.add_fluid_diagnostics(name='FluidDiagnostics2',
                                   diag_type='flux_s',
                                   write_every=10,
                                   compute_every=5,
                                   start_iteration=0,
                                   last_iteration=1000,
                                   species_name='proton1')

        simu.add_electromag_diagnostics(name='ElectromagDiagnostics1',diag_type='E',
                                        write_every=10,compute_every= 5,
                                        start_iteration= 0, last_iteration=1000)

        simu.add_electromag_diagnostics(name='ElectromagDiagnostics2',diag_type='B',
                                        write_every=20, compute_every=10,
                                        start_iteration= 0,last_iteration= 1000)

        model = UniformModel()
        model.add_fields()
        model.add_species("proton1")
        model.add_species("proton2", density=2.)

        simu.add_model(model)

        simu.write_ini_file()


        config=configparser.ConfigParser()
        config.read("phare.ini")
        sections = config.sections()
        expected_sections=('Simulation','model','FluidDiagnostics1',
                            'FluidDiagnostics2',
                            'ElectromagDiagnostics1',
                            'ElectromagDiagnostics2')

        for section in sections:
            self.assertIn(section, expected_sections)

        numOfCellx = config.getint('Simulation', 'numOfCellx')
        numOfCelly = config.getint('Simulation', 'numOfCelly', fallback=0)
        numOfCellz = config.getint('Simulation', 'numOfCellz', fallback=0)

        cells = [numOfCellx, numOfCelly, numOfCellz]
        self.assertEqual(cells,[80,0,0])


        dx = config.getfloat('Simulation', 'dx')
        dy = config.getfloat('Simulation', 'dy', fallback=0)
        dz = config.getfloat('Simulation', 'dz', fallback=0)

        dl = [dx, dy, dz]
        self.assertEqual(dl,[0.1,0,0])

        Ox = config.getfloat('Simulation', 'originx', fallback=0)
        Oy = config.getfloat('Simulation', 'originy', fallback=0)
        Oz = config.getfloat('Simulation', 'originz', fallback=0)

        origin = [Ox, Oy, Oz]
        self.assertEqual(origin,[0.,0.,0.])

        bc = config.get('Simulation', 'boundaryConditionx')
        self.assertEqual('periodic', bc)

        pusher = config.get('Simulation', 'particlePusher')
        self.assertEqual('modifiedBoris', pusher)

        splitting = config.get('Simulation', 'splittingMethod')
        self.assertEqual('splitOrderN_RF2', splitting)
###############################################################################



def prepare_job(simulation):
    """
    prepare a simulation for a run:
        - creates the simulation directory [simulation.path] if it does not exist yet
        - writes an INI file in the directory [simulation.path]
        - for each diagnostic registered in the simulation:
            - creates the diagnostic directory 'diag['path']' under [simulation.path]/
              if it does not exist yet
    """

    diagnostics = simulation.em_diagnostics + simulation.fluid_diagnostics

    print("preparing job...")
    if not os.path.exists(simulation.path):
        print("mkdir "+simulation.path)
        os.makedirs(simulation.path)
        print("writing ini file in " + simulation.path+'/')
        simulation.write_ini_file()

    for diag in diagnostics:
        path = diag["path"]
        full_path = os.path.join(simulation.path, path)
        if not os.path.exists(full_path):
            print("mkdir " + full_path)
            os.makedirs(full_path)



###############################################################################
#
#                   usage example and run tests
#
#       the script: - creates a Simulation
#                   - registers 2 fluid diagnostics to the Simulation
#                   - registers 2 electromag diagnostics to the Simulation
#                   - creates a model 'UniformModel'
#                   - adds default electromagnetic fields to the model
#                   - adds 2 proton species, the second has a density of 2.
#                   - registers the model to the Simulation
#                   - prepare the job directories and input file
#
#                   - executs unit tests of this module
###############################################################################
if __name__ == '__main__':

     simu = Simulation(time_step_nbr=1000, boundary_types="periodic",
                       cells=(80,0,0), dl=(0.1,0,0),
                       final_time=1., path='test')

     simu.add_fluid_diagnostics(name='FluidDiagnostics1',
                                diag_type='rho_s', write_every=10, compute_every=5,
                                start_iteration=0, last_iteration=1000,
                                species_name='proton1')

     simu.add_fluid_diagnostics(name='FluidDiagnostics2',
                                diag_type='flux_s', write_every=10, compute_every=5,
                                start_iteration=0, last_iteration=1000,
                                species_name= 'proton1')

     simu.add_electromag_diagnostics(name='ElectromagDiagnostics1',diag_type='E',
                                     write_every=10, compute_every=5,
                                     start_iteration=0, last_iteration= 1000)

     simu.add_electromag_diagnostics(name='ElectromagDiagnostics2',diag_type='B',
                                     compute_every=10, write_every= 20,
                                     start_iteration= 0, last_iteration= 1000)

     model = UniformModel()
     model.add_fields()
     model.add_species("proton1")
     model.add_species("proton2", density=2.)

     simu.add_model(model)

     prepare_job(simu)

     unittest.main()
