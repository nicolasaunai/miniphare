#!/usr/bin/env python

import unittest
import numpy as np
import configparser
import os


class UniformModel(object):

    def __init__(self):
        self.model = {"model":"model","model_name":"uniform"}


    def add_fields(self,B=(1.,0.,0.),E=(0.,0.,0.)):
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

    def add_species(self,name,
                    charge=1,
                    mass=1,
                    nbrPartCell=100,
                    density=1.,
                    vbulk=(0.,0.,0.),
                    temperature=0.0):

        keys = self.model.keys()
        nbr_species = 0
        for k in keys:
            if k[:7] == 'species':
                nbr_species+=1

        idx=str(nbr_species)

        new_species = {"speciesName"+idx:name,
                       "charge"+idx:charge,
                       "mass"+idx:mass,
                       "density"+idx:density,
                       "vbulk"+idx:vbulk,
                       "temperature"+idx:temperature,
                       "nbrParticlesPerCell"+idx:nbrPartCell}

        keys = self.model.keys()
        if "speciesName"+idx in keys:
            raise ValueError("species already registered")

        self.model.update(new_species)


    def to_dict(self):
        return self.model


def add_dict_to_config(the_dict,conf, section_key="section"):
    sections = conf.sections()
    this_section = the_dict[section_key]
    if this_section not in sections:
        conf.add_section(this_section)
    for key in the_dict.keys():
        if key != section_key:
            conf.set(the_dict[section_key], key, str(the_dict[key]))





class Simulation(object):
    """represents a simulation input"""

    fluid_diag_types = ['rho_s', 'flux_s']
    em_diag_types=['E', 'B']


    def __init__(self, time_step_nbr,
                 boundary_types,
                 cells,
                 domain_size=None,
                 dl=None,
                 time_step=None,
                 final_time=None,
                 layout="yee",
                 interp_order=1,
                 nbr_ion_populations=1,
                 particle_pusher="modifiedBoris",
                 splitting_method="splitOrderN_RF2",
                 origin=(0,0,0),
                 path='.'):

        super(Simulation, self).__init__()

        self.path = path

        def missingInfo(a,b):
            if a is None or b is None:
                raise ValueError("not enough information to build domain")
            if not (isinstance(a,list) or isinstance(a,tuple)) or not \
            (isinstance(b,tuple) or isinstance(b,list)):
                raise ValueError("not iterable")


        if domain_size is None:
            missingInfo(cells, dl)
            self.domain_size = [cell*d for cell,d in zip(cells,dl)]
            self.cells = cells
            self.dl = dl

        elif dl is None:
            missingInfo(domain_size,cells)
            self.dl= []
            for L, n in zip(domain_size, cells):
                if n == 0:
                    self.dl.append(-1)
                else:
                    self.dl.append(L/float(n))
            self.cells = cells
            self.domain_size = domain_size


        self.time_step_nbr = time_step_nbr
        if final_time is not None and time_step is not None:
            raise ValueError("specify either time_step or final_time not both")
        if final_time is not None:
            self.time_step = final_time/float(time_step_nbr)
            self.final_time = final_time
        else:
            if time_step is not None:
                self.final_time = time_step_nbr * time_step
                self.time_step = time_step
            else:
                raise ValueError("specify time_step or final_time")

        # search dimension
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


        if interp_order not in [1,2,3,4]:
            raise ValueError("invalid interpolation order")
        else:
            self.interp_order = interp_order


        error_bc_dim = "boundary_types must have {} dimension".format(self.dims)
        if isinstance(boundary_types,list) or isinstance(boundary_types,tuple):
            if self.dims == len(boundary_types):
                self.boundary_types = boundary_types
            else:
                raise ValueError(error_bc_dim)
        else:
            if self.dims==1:
                self.boundary_types = []
                self.boundary_types.append(boundary_types)
            else:
                print(self.cells, boundary_types)
                raise ValueError(error_bc_dim)


        self.nbr_ion_populations = nbr_ion_populations
        self.origin = origin
        self.layout = layout
        self.splitting_method = splitting_method

        if particle_pusher not in ["modifiedBoris"]:
            raise ValueError("invalid pusher")
        else:
            self.particle_pusher = particle_pusher


        self.fluid_diagnostics=[]
        self.em_diagnostics=[]
        self.particle_diagnostics=[]


    def add_fluid_diagnostics(self, name, diag_type,
                              write_every, compute_every,
                              istart, iend, species_name, path=None):

            if path is None:
                path = name.lower()

            if diag_type not in Simulation.fluid_diag_types:
                raise ValueError("invalid diagnostic type : {}".format(diag_type))

            if write_every < compute_every:
                raise ValueError("write_every should be >= compute_every")

            if istart >= iend:
                raise ValueError("iStart should be < iEnd")

            for diag in self.fluid_diagnostics:
                if name == diag["name"]:
                    raise ValueError("diagnostic name already used")


            self.fluid_diagnostics.append({"name":name,
                                    "diagCategory":"FluidDiagnostics",
                                    "diagType":diag_type,
                                    "writeEvery":write_every,
                                    "computeEvery":compute_every,
                                    "iStart":istart,
                                    "iEnd":iend,
                                    "speciesName":species_name,
                                    "path":path})

    def add_electromag_diagnostics(self, name, diag_type,
                              write_every, compute_every,
                              istart, iend, path=None):

            if path is None:
                path = name.lower()

            if diag_type not in Simulation.em_diag_types:
                raise ValueError("invalid diagnostic type : {}".format(diag_type))

            if write_every < compute_every:
                raise ValueError("write_every should be >= compute_every")

            if istart >= iend:
                raise ValueError("iStart should be < iEnd")

            for diag in self.em_diagnostics:
                if name == diag["name"]:
                    raise ValueError("diagnostic name already used")

            self.em_diagnostics.append({"name":name,
                                    "diagCategory":"ElectromagDiagnostics",
                                    "diagType":diag_type,
                                    "writeEvery":write_every,
                                    "computeEvery":compute_every,
                                    "iStart":istart,
                                    "iEnd":iend,
                                    "path":path})



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



    def add_model(self,model):
        self.model = model


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
            j = Simulation(self.time_step_nbr, bc, cells ,dl=dl, final_time=self.final_time)
            for d in np.arange(dim):
                self.assertEqual(j.domain_size[d], j.dl[d]*cells[d])


    def test_dl(self):
        for cells,domain_size,dim, bc in zip(self.cells_array,
                                             self.domain_size_array,
                                             self.dims,
                                             self.bcs):
            j = Simulation(self.time_step_nbr, bc, cells ,
                           domain_size=domain_size, final_time=self.final_time)
            for d in np.arange(dim):
                self.assertEqual(j.dl[d], domain_size[d]/float(cells[d]))



    def test_boundary_conditions(self):
        j = Simulation(1000, "periodic", (80,0,0) ,domain_size=(10,0,0), final_time=1.)
        for d in np.arange(j.dims):
            self.assertEqual("periodic", j.boundary_types[d])


    def test_assert_boundary_condition(self):
        j= Simulation(1000, "periodic", (80,0,0) ,domain_size=(10,0,0), final_time=1000)


    def test_final_time(self):
        s = Simulation(1000,"periodic",(80,0,0), domain_size=(10,0,0), time_step=0.01)
        self.assertEqual(10,s.final_time)

    def test_time_step(self):
        s = Simulation(1000,"periodic",(80,0,0), domain_size=(10,0,0), final_time=10)
        self.assertEqual(0.01,s.time_step)



    def test_ini_file(self):
        """ serialize a simulation in an INI file and read it to check it is OK"""

        simu = Simulation(1000,"periodic",(80,0,0),dl=(0.1,0,0), final_time=1.)
        simu.add_fluid_diagnostics('FluidDiagnostics1','rho_s', 10, 5, 0, 1000, 'proton1')
        simu.add_fluid_diagnostics('FluidDiagnostics2','flux_s', 10, 5, 0, 1000, 'proton1')
        simu.add_electromag_diagnostics('ElectromagDiagnostics1','E', 10, 5, 0, 1000)
        simu.add_electromag_diagnostics('ElectromagDiagnostics2','B', 20, 10, 0, 1000)

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




if __name__ == '__main__':

     simu = Simulation(1000,"periodic",(80,0,0),dl=(0.1,0,0), final_time=1.,path='test')
     simu.add_fluid_diagnostics('FluidDiagnostics1','rho_s', 10, 5, 0, 1000, 'proton1')
     simu.add_fluid_diagnostics('FluidDiagnostics2','flux_s', 10, 5, 0, 1000, 'proton1')
     simu.add_electromag_diagnostics('ElectromagDiagnostics1','E', 10, 5, 0, 1000)
     simu.add_electromag_diagnostics('ElectromagDiagnostics2','B', 20, 10, 0, 1000)

     model = UniformModel()
     model.add_fields()
     model.add_species("proton1")
     model.add_species("proton2", density=2.)

     simu.add_model(model)

     prepare_job(simu)

     unittest.main()
