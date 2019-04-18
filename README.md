# AfpAnalysisToolbox_example

In order to run an example AFP analysis log in to `lxplus` and do:
```bash
mkdir MyAfpAnalysis
cd MyAfpAnalysis
git clone --recursive https://:@gitlab.cern.ch:8443/afp/AfpAnalysisToolbox_example.git source
mkdir build run
cd build
setupATLAS
asetup 21.2.70,AnalysisBase
cmake ../source/
make
source */setup.sh
cd ../run
runAfpAnalysisExample
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

You should run the code from `run`. Remember to either remove `submitDir` created in a previous execution or to specify a new directory
```bash
cd ../run
runAfpAnalysisExample submitDir1
```
You can also use Python steering macro:
```bash
runAfpAnalysisExample.py -s submitDir2
```


Every time you log in you need to do:
```bash
setupATLAS
cd MyAfpAnalysis/build
asetup --restore
source */setup.sh
```
A tutorial on xAOD analysis can be found in https://atlassoftwaredocs.web.cern.ch/ABtutorial

In case of problems, questions or suggestions, please do not hesitate to send an e-mail to e-group [atlas-afp-analysis-software@cern.ch](mailto:atlas-afp-analysis-software@cern.ch).
