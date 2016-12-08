# MiniPhare sequence
## Getting initial parameters
MiniPHARE can in principle be transparently started from multiple input formats, that are recognized by their file extension (*.py, *.txt, ...). 

MiniPHARE starts with executing ```fromCommandLine(argc, argv)```. This function checks the command line arguments and returns a concrete ```InitializerFactory```.  ```InitializerFactory``` is an abstract factory that returns factories for the main modules in the code, Ions, Solvers, Electromag, etc. There is one concrete ```InitializerFactory``` for each input file format available in MiniPHARE. 

For instance:
```
 ./miniphare init.py
 ```
 
Will make ```fromCommandLine()``` to return a ```PythonInitializerFactory```, which knows how to return factories from parameters read in a python script.

So far no input file format is available but since the rest of the code only knows about the abstract ```InitializerFactory``` it is independent of the specific format we use.

For now, only the ```SimpleInitializerFactory``` is coded. It is a simple factory that hard-codes everything so to get rid of the trouble of dealing with python or other external input file format. The ```SimpleInitializerFactory``` defines ion species with a ```FluidParticleInitializer``` setup with hard-coded fluid profiles profiles.


## Setting up the MLMD root level

Now the code knows how to interact with the input file format thanks to its concrete ```InitializerFactory```. The next thing to do is to setup the root level of the MLMD system and its only patch ```root```. By setting up the root level, we mean, initializing the fields and the particles and other numerical parameters such a the mesh properties etc.

The MLMD system is made of a ```Hierarchy```, which contains a ```root``` ```Patch```. This root patch, like any other patch, contains a ```PatchData``` object, holding its data (Ions, fields, solver etc.) and a vector of ```Patch``` children.  Setting up the MLMD system therefore means we have to setup the ```PatchData``` of the root patch of the Hierarchy.  We simply pass the concrete ```InitializerFactory``` to the PatchData so it can get the information needed to initialize its fields and particles etc.


### Initializing the root level
We now need to initialize the patch data of the root level. MiniPhare therefore now calls ```MLMD::initializeRootLevel()```, which calls ```Patch::init()``` for its single root patch. This function takes the ```PatchData``` of the root Patch and calls ```PatchData::init()```, which can now initialize all its constituants: electromagnetic fields (using the ```ElectromagInitializer``` returned by the ```InitializerFactory```),  the Solver (using the ```SolverInitializer``` return by the ```InitializerFactory```), the Ions (using the ```IonInitializer``` returned by the ```InitializerFactory``` ).


### Initializing the ions
The ```PatchData``` of the ```root``` ```Patch``` of the ```Hierarchy``` calls ```Ions::loadParticles()``` to initialize the particles of all its species. This function is quite simple, it just loops over all ion species and calls ```Species::loadParticles()```.  When ```Ions``` have been built, it has created all of its species, giving them their own ```ParticleInitializer``` returned from the ```IonInitializer```.  Each species therefore knows how to load its own particles and this depends on the specific ```ParticleInitializer```.  One often use case of ```ParticleInitializer``` is the ```FluidParticleInitializer```,  which loads particles following user supplied density, velocity and thermal speed spatial profiles, known from the ```InitializerFactory```.
