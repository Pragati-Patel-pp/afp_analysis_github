# AfpAnalysisToolbox_example

In order to run an example AFP analysis log in to `lxplus` and do:
```bash
mkdir MyAfpAnalysis
cd MyAfpAnalysis
git clone --recursive https://github.com/Pragati-Patel-pp/afp_analysis_github.git source
mkdir build run
cd build
setupATLAS
asetup 24.2.35,AnalysisBase
cmake ../source/
make
source */setup.sh
cd ../run
runAfpAnalysisExample.py
```

The analysis code is located in `source/AfpAnalysis/`. If you modify it and want to recompile you need to go to the `build` directory
```bash
cd ../build
```
and run
```bash
cmake ../source
make
```

You should run the code from `run` directory. Remember to either remove `submitDir` created in a previous execution or to specify a new directory
```bash
cd ../run
runAfpAnalysisExample.py -s submitDir1
```

Every time you log in you need to do:
```bash
setupATLAS
cd MyAfpAnalysis/build
asetup --restore
source */setup.sh
```
A tutorial on xAOD analysis can be found in https://atlassoftwaredocs.web.cern.ch/ABtutorial

To update AfpAnalysisToolbox to newest version:
```bash
cd MyAfpAnalysis/source/AfpAnalysisToolbox
git stash # save local changes if any
git pull origin master
```
then recompile code from `build` directory.

In case of problems, questions or suggestions, please do not hesitate to send an e-mail to e-group [atlas-afp-analysis-software@cern.ch](mailto:atlas-afp-analysis-software@cern.ch).
