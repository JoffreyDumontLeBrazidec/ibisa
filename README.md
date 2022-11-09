# IBISA

IBISA (Inverse Bayesian Inference for Source Assessment) describes the sampling of radionuclide release sources using the Reversible Jump MCMC algorithm.

## Installation

Use the command
scons
in the main directory to compile and create an executable "mcmc"

```bash
scons
```

## Usage

```bash
./mcmc myconfiguration/config-conc.cfg
```

## Description

IBISA is written in .cpp and organised in classes.
Each class is located in a file with the same name, in a folder with the same name.
Each folder is described below.

configuration: a typical configuration file can be found inside. In particular, the paths must be changed.

controller: list of std::exception() to check the validity of each configuration parameter and facilitate bug recovery.

state: MarkovState class containing information about a Markov state (stage of the Markov chain)

cost: Cost class whose main role is to calculate the cost relative to a Markov state.

managers: the managers store, alpha, and cycle organise the storage of the MCMC results, the acceptance ratios of the MCMC algorithm, and the update cycle of the Markov chain, respectively.

sortingobs: the role of the sortingObs class is to organise the sorting of observations into relevant or irrelevant.

observationOperator : the role of the observationOperator class is to load, hold, and return information about the observation operator matrix.

redoperator : the ReducDim_Obs_Operator class is called by observationOperator and is used instead when transdimensional walks are allowed.

storage: the Storage class is used to store the results of the reversible jump MCMC algorithm.

main: the paralleltempering.cpp file is used to call the MCMC reversible jump algorithm and describes its cycle.


## Authors and acknowledgment

This code corresponds to the article 
Bayesian transdimensional inverse reconstruction of the 137Cs Fukushima-Daiichi release
by Joffrey Dumont Le Brazidec, Olivier Saunier, Marc Bocquet, and Yelva Roustan
and was developed as part of a PhD at 
the IRSN (Institut de radioprotection et de sûreté nucléaire), Fontenay-aux-Roses, France
and the CEREA (Centre d'Enseignement et de Recherche en Environnement Atmosphérique) at the Ecole des Ponts and EDF R&D, Île de France, France

## Support

Contact joffrey.dumont@enpc.fr

## License

[MIT](https://choosealicense.com/licenses/mit/)
