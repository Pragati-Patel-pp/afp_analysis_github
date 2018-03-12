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
job.options().setDouble( ROOT.EL.Job.optMaxEvents, 2 )

# configure algorithm
from AnaAlgorithm.AnaAlgorithmConfig import AnaAlgorithmConfig
config = AnaAlgorithmConfig( 'AfpAnalysisExample')

# # ===== OPTIONAL CONFIGURATION =====
# # --- create main analysis tool ---
# config.addPrivateTool ("afpTool", "AFP::AfpAnalysisTool")
# config.afpTool.hitsContainerName = "AFPSiHitContainer"
# config.afpTool.clusterContainerName = "AFPSiHitsClusterContainer"
# config.afpTool.tracksContainerName = "AFPTrackContainer"
# config.afpTool.protonsContainerName = "my_proton_name_2"

# # --- create and configure proton reconstruction tool ---
# config.addPrivateTool ("afpTool.clusterTool", "AFP::ClusteringTool")

# # --- create and configure track finding tool ---
# config.addPrivateTool ("afpTool.trackFindTool", "AFP::TrackFindSimpleClusteringTool")
# config.afpTool.trackFindTool.allowedDistanceBetweenClustersInTrack = 42  # set distance between tracks

# # --- create and configure track reconstruction tool ---
# config.addPrivateTool ("afpTool.trackRecoTool", "AFP::TrackRecoLinRegTool")

# # --- create and configure proton reconstruction tool ---
# config.addPrivateTool ("afpTool.protonRecoTool", "AFP::ProtonRecoChi2Minuit")
# # config.addPrivateTool ("afpTool.protonRecoTool", "AFP::ProtonRecoAnalytical")
# # config.afpTool.protonRecoTool.parametrizationFileNameBeam1 = "parameterization_beta_0.40_b1_2017.txt"
# # config.afpTool.protonRecoTool.parametrizationFileNameBeam2 = "parameterization_beta_0.40_b2_2017.txt"

# # &&&&& END OPTIONAL CONFIGURATION &&&&&



job.algsAdd( config )

driver = ROOT.EL.DirectDriver()
driver.submit( job, options.submission_dir )

