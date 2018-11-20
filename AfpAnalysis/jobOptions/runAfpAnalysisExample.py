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
inputFilePath = "/eos/atlas/atlascerngroupdisk/det-afp/AnalysisData/Tutorial/data17_13TeV.00331975.physics_Main.merge.AOD.r10203_p3399/"
ROOT.SH.ScanDir().filePattern( '*' ).scan( sh, inputFilePath )

job = ROOT.EL.Job()
job.sampleHandler( sh )
job.options().setDouble( ROOT.EL.Job.optMaxEvents, 10 )

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

# Side C
# config.afpTool.protonRecoToolSide1.parametrizationFileName = "parameterization_beta_0.40_b1_2017.txt"

# &&&&& END OPTIONAL CONFIGURATION &&&&&


job.algsAdd( config )

driver = ROOT.EL.DirectDriver()
driver.submit( job, options.submission_dir )

