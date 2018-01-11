mkdir run
mkdir build

cd source 
git clone https://:@gitlab.cern.ch:8443/afp/AfpAnalysisToolbox.git
svn export svn+ssh://svn.cern.ch/reps/atlasoff/ForwardDetectors/FPTracker/trunk FPTracker

cd ../build/
asetup 21.2,AnalysisBase,latest

cmake ../source/
make

source */setup.sh

