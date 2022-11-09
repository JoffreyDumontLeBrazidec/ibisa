# IBISA

The goal of IBISA (Inverse Bayesian Inference for Source Assessment) is to provide an algorithm to
sample radionuclide release sources.



## Installation

Use the command
```bash
scons
```
in the main directory to compile and create an executable "mcmc"


## Usage

```bash
./mcmc myconfiguration/config-conc.cfg
```

##  Description

This repository is organised as follows:
- The preprint corresponding to this code can be found here: (https://gmd.copernicus.org/preprints/gmd-2022-168/#discussion).
- The source code is located in the `src/` folder. The `.cpp` files together
implement the `mcmc` module. 

Each class is located in a file with the same name, in a folder with the same name.
Each folder is described below:

- configuration: contains a typical configuration file (in particular, paths must be changed).

- controller: contains a list of std::exception() to check the validity of each configuration parameter and facilitate bug recovery.

- state: contains MarkovState class used to treat information about a Markov state (stage of the Markov chain)

- cost: contains Cost class used to calculate the cost relative to a Markov state.

- managers: contains the managers store, alpha, and cycle used to organise the storage of the MCMC results, the acceptance ratios of the MCMC algorithm, and the update cycle of the Markov chain, respectively.

- sortingobs: contains the sortingObs class used to organise the sorting of observations into relevant or irrelevant.

- observationOperator: contains the observationOperator class used to load, hold, and return information about the observation operator matrix.

- redoperator: contains the ReducDim_Obs_Operator class called by observationOperator and used instead when transdimensional walks are allowed.

- storage: contains the Storage class used to store the results of the reversible jump MCMC algorithm.

- main: contains the paralleltempering.cpp file used to call the MCMC reversible jump algorithm and describes its cycle.

##  Authors and acknowledgment

This code goes along the article:__
"Bayesian transdimensional inverse reconstruction of the 137Cs Fukushima-Daiichi release"__
Joffrey Dumont Le Brazidec, Olivier Saunier, Marc Bocquet, and Yelva Roustan__

It was developed as part of a PhD at 
the IRSN (Institut de radioprotection et de sûreté nucléaire), Fontenay-aux-Roses, France
and the CEREA (Centre d'Enseignement et de Recherche en Environnement Atmosphérique) at the Ecole des Ponts and EDF R&D, Île de France, France

##  Support

Contact joffrey.dumont@enpc.fr

## License

[MIT](https://choosealicense.com/licenses/mit/)
