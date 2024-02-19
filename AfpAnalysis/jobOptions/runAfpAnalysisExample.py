#!/usr/bin/env python

import optparse
parser = optparse.OptionParser()
parser.add_option( '-s', '--submission-dir', dest = 'submission_dir',
    action = 'store', type = 'string', default = 'submitDir',
    help = 'Submission directory for EventLoop' )
( options, args ) = parser.parse_args()

import ROOT
ROOT.xAOD.Init().ignore()

import os
sh = ROOT.SH.SampleHandler()
sh.setMetaString( 'nc_tree', 'CollectionTree' )

#inputFilePath = "/eos/user/p/prpatel/data22_13p6TeV.00428770.physics_Main.deriv.DAOD_PHYS.r13928_p5279_p5858/"
#ROOT.SH.ScanDir().filePattern('*.1' ).scan( sh, inputFilePath )
#ROOT.SH.ScanDir().filePattern('DAOD_PHYS.34860090._000003.pool.root.1' ).scan( sh, inputFilePath )

inputFilePath = "/eos/user/p/prpatel/data22_13p6TeV.00428770.physics_MinBias.merge.AOD.r14956_p5604_tid34714990_00"
ROOT.SH.ScanDir().filePattern('AOD.34714990._000013.pool.root.1' ).scan( sh, inputFilePath )

#ROOT.SH.scanRucio(sh,"data22_13p6TeV.00428770.physics_MinBias.merge.AOD.r14956_p5604_tid34714990_00/")
#sh.setMetaString('nc_grid_filter','AOD.')


job = ROOT.EL.Job()
job.sampleHandler( sh )
job.options().setDouble( ROOT.EL.Job.optMaxEvents, -1 )
#job.options().setString(ROOT.EL.Job.optSubmitFlags,'--match="*"')

out = ROOT.EL.OutputStream( "skim" , "xAOD" )
job.outputAdd ( out )
# configure algorithm
from AnaAlgorithm.AnaAlgorithmConfig import AnaAlgorithmConfig
config = AnaAlgorithmConfig( 'AfpAnalysisExample')

# ===== OPTIONAL CONFIGURATION =====
# --- create main analysis tool ---
config.addPrivateTool ("afpTool", "AFP::AfpAnalysisTool")
config.afpTool.hitsContainerName = "AFPSiHitContainer"
config.afpTool.clusterContainerName = "AFPSiHitsClusterContainer"
config.afpTool.tracksContainerName = "AFPTrackContainer"
config.afpTool.protonsContainerName = "AFPProtonContainer"



# --- create and configure proton reconstruction tool for each side ---
### ProtonRecoAnalytical:
config.addPrivateTool ("afpTool.protonRecoToolSide0", "AFP::ProtonRecoAnalytical")
config.addPrivateTool ("afpTool.protonRecoToolSide1", "AFP::ProtonRecoAnalytical")

# Side A
# config.afpTool.protonRecoToolSide0.parametrizationFileName = "parameterization_beta_0.40_b2_2017.txt"
# config.afpTool.protonRecoToolSide0.allowSingleStationReco = True

# Side C
# config.afpTool.protonRecoToolSide1.parametrizationFileName = "parameterization_beta_0.40_b1_2017.txt"
# config.afpTool.protonRecoToolSide1.allowSingleStationReco = True

# &&&&& END OPTIONAL CONFIGURATION &&&&&


job.algsAdd( config )

driver = ROOT.EL.DirectDriver()
driver.submit( job, options.submission_dir )
#driver=ROOT.EL.PrunDriver()
#driver.options().setDouble(ROOT.EL.Job.optGridNFilesPerJob, 5)
#driver.options().setDouble(ROOT.EL.Job.optGridMaxNFilesPerJob, 5)
#driver.options().setString('nc_grid_filter','*')

#driver.options().setString("nc_outputSampleName", "user.prpatel.%in:name[2]%.%in:name[6]%.all_triggers.v0");
#driver.submitOnly(job,submission_dir)


