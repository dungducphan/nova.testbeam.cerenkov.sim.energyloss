# Energy Loss at Cerenkov Counter

A Simulation for the Study of Energy Loss due to Interactions of Tertiary Beam Particles to the CO2 Cerenkov Threshold Counter in the NOvA Testbeam Project in Fermilab

## Goals:

We want to study the energy loss due to the interactions with the Cerenkov counter’s materials.
- A simple simulation configuration.
- How many percents of the beam particles can become signal?
- Energy loss as function of energy and particle types.

## Simulation Configuration

### Geometry
We made a simulation to understand the effect of the detector’s materials on the beam of particles.  It includes
- A 2.5m-long(in the beam direction) CO2 volume at a density 1.84E-3 g/cm2.
- Two Kapton windows, each of 1 mm thick, density of 1.42 g/cm2.
- Two fake 13-by-13 cm2 Time-Of-Flight's (TOFs), each of 1 mm thick, made of air. These two volumes is set to be `SensitiveDetector` and will` record the energy of the primary particles (beam particles) when they are passing through.
- A mylar mirror, 2 mm in thickness, density of 1.4 g/cm2.
- World volume made of air, 5-m long in the beam direction.

Detector components


![alt text](screenshots/eventDisplay_annotated.png "Detector components in the simulation.")

### Physics
- Particle Transport.
- Energy loss.
- EM (scattering processes are in here).
- Particle Decay.
- Optical Photon Processes are included but due to lack of definition of `OpticalPropertiesTable`, these processes become dummies.

### Analysis
- `ROOT` is used instead of `G4Analysis`.
- `AnalysisTree` is created at the beginning of every `Run`. The tree just includes two variables: `Energy` and `EnergyLoss`, in which `Energy` is the initial energy of the particle when it enters the world volume and `EnergyLoss` is the energy loss due to the interactions of the particle with the detector's components. One event is corresponded to only one primary particle.
- The TOF modules are set to be `SensitiveDetector`. They create a `G4Hit` storing the current kinetic energy per every hit the particle makes in their physical volume and then add the `G4Hit` into a `G4HitCollection` for every event. At the end of an event, we extract the hits in the `G4HitCollection` to calculate the energy loss and fill the `AnalysisTree` with the calculated value. At the end of an event, the `G4HitCollection` is deleted.
- Notice that a particle can rarely make two or more hits in the same TOF module in an event. In such a case, we take the initial energy as the maximum energy in the TOF upstream (TOF_US) and the final energy as the minimum energy in the TOF downstream (TOF_DS).
- We ran the simulation with different types of particles (electron, muon, gamma, proton, neutron, pion-minus, pion-zero, kaon-minus and kaon-zeros). For each type of particle, we ran simulation with different energies (10~MeV - 10~GeV, 60 bins in log-scale). And for each energy bin, we shoot in 10000 primary particles (or running 10000 events). Particles are required to pass through both TOFs to be analyzed. So, when shooting 10000 particles, not all of these primaries can reach the TOF\_DS to become a signal.
- Post-production analysis `ROOT` scripts are provided. See instruction below.

## Building and Usage

### Prerequisite
- Setup `ROOT` environment. If you don't have a `ROOT` environmant installed, see [Building ROOT](https://root.cern.ch/building-root) for more details.
- Setup `Geant4` environment. If you don't have a `Geant4` environmant installed, see [Geant4 Installation Guide](http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/InstallationGuide/html/index.html) for more details.
- `CMake`, `git` are required.

### Building
- Cloning the repo. Using the tag `v1.1`.
```sh
    git clone https://github.com/dungducphan/nova.testbeam.cerenkov.sim.energyloss.git
    git checkout tags/v1.1
```
- Build. We assume the convention of having `build` directory as the same level as `source` directory.
```sh
    mkdir build
    cd build
    cmake ../nova.testbeam.cerenkov.sim.energyloss
    make
```

### Production
First, we need to make the macro files.
- Open the script `MakeMacro.C` to review and/or change the production run configurations.
- The number of events for each particle and an energy range is the global variable `NumberOfEvent` (line 7 with tag v1.1).
```c_cpp
    unsigned int NumberOfEvent = 10000;
```
- The particle list is defined in lines [34-43]. If you want to add more, please see the particle name convention for `Geant4` [here](http://fismed.ciemat.es/GAMOS/GAMOS_doc/GAMOS.5.0.0/x11174.html).
```c_cpp
    std::vector<std::string> ParticleList;
    ParticleList.push_back("e-");
    ParticleList.push_back("mu-");
    ParticleList.push_back("gamma");
    ParticleList.push_back("proton");
    ParticleList.push_back("neutron");
    ParticleList.push_back("pi0");
    ParticleList.push_back("pi-");
    ParticleList.push_back("kaon0");
    ParticleList.push_back("kaon-");
```
- Energy bins for the production run is define in lines [45-46]
```c_cpp
    std::vector<double> E     = LogBins(60, 1.E1, 1.E4);
    std::vector<double> Eloss = LogBins(60, 1.E1, 1.E3);
```
- Run the script `MakeMacro.C` to make the macro files. The macros will be in `Macros` folder.
```sh
root -l -bq MakeMacro.C
```

Once, we ran the `MakeMacro.C`, not only the macros are created, the bashcript `run.sh` is also made available.
- Run this script to start the run
```sh
bash ./run.sh
```
- It would take quite a while for the production run to finish. Take your time.
- After the run finished, the `.root` files will be saved in `Output` folder and ready for analysis.
- **If you ever need to clean up everything** to start a run production or new analysis batch, run the `clean.sh` script.
```sh
bash ./clean.sh
```

### Analysis
- First step in doing analysis is joining the `.root` files that corresponds to the same particle type.
```sh
bash ./join.sh
```
- All the joined `.root` will be saved in the `Results` folder.
- Now, it comes to you to write your analysis scripts to analyze the data. We did provided `Analysis.C` to perform 2 simple analyses:
    + Energy loss as a function of energy for each type of particle.
    + Fraction of signal for each type of particle.
- Run the script with 2 arguments, the first is a string for particle name (following `Geant4` name convention), and the second is the total number of particles in a single run (see `NumberOfEvents` variable in `MakeMacros.C` to find this number).
```sh
root -l -bq 'Analysis.C("e-", 10000)'
```
- The analysis plots will be saved in `Results` folder.

### Some money plots

An event display for a run of 100 electrons at 250 MeV. Scattering made the electron beams spread out in the TOF_DS.


![alt text](screenshots/eventDisplay_e.png "Event Display")


Energy loss of electrons


![alt text](screenshots/eloss_e.png "Energy loss for electrons.")


Energy loss of muons


![alt text](screenshots/eloss_mu.png "Energy loss for muons.")


Fraction of signal for electrons


![alt text](screenshots/signal_e.png "Signal fraction for electrons.")


Fraction of signal for muons


![alt text](screenshots/signal_mu.png "Signal fraction for muons.")
