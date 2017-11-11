
import configparser
import sys
import os

def main():
    config = configparser.ConfigParser()

    pathname = sys.argv[1]

    config['simulation'] = {"numOfCellx":80,
                            "numOfCelly":0,
                            "numOfCellz":0,
                            "dx":0.1,
                            "dy":0.0,
                            "dz":0.0,
                            "originx":0,
                            "originy":0,
                            "originz":0,
                            "boundaryConditionx":"periodic",
                            "boundaryConditiony":"periodic",
                            "boundaryConditionz":"periodic",
                            "layout":"yee",
                            "interpOrder":1,
                            "nbrIonPopulations":2,
                            "timeStep":0.001,
                            "numOfSteps":1000,
                            "particlePusher":"modifiedBoris",
                            "splittingMethod":"splitOrderN_RF2"
                            }

    config['model'] = { "model_name":"uniform",
                        "speciesName0":"proton1",
                        "nbrParticlesPerCell0":100,
                        "charge0":1,
                        "mass0":1,
                        "density0":0.5,
                        "vbulkx0":0.0,
                        "vbulky0":0.0,
                        "vbulkz0":0.0,
                        "temperature0":0.0,
                        "speciesName1":"proton2",
                        "nbrParticlesPerCell1":100,
                        "charge1":1,
                        "mass1":1,
                        "density1":0.5,
                        "vbulkx1":0.0,
                        "vbulky1":0.0,
                        "vbulkz1":0.0,
                        "temperature1":0.0,
                        "bx":1.0,
                        "by":1.0,
                        "bz":1.0,
                        "ex":0.0,
                        "ey":0.0,
                        "ez":0.0}

    with open(os.path.join(pathname,"phare.ini"), "w") as configfile:
         config.write(configfile)



if __name__ == '__main__':
    main()
