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

# ===== OPTIONAL CONFIGURATION =====
# --- create main analysis tool ---
config.addPrivateTool ("afpTool", "AFP::AfpAnalysisTool")
config.afpTool.hitsContainerName = "AFPSiHitContainer"
config.afpTool.clusterContainerName = "AFPSiHitsClusterContainer"
config.afpTool.tracksContainerName = "AFPTrackContainer"
config.afpTool.protonsContainerName = "AFPProtonContainer"
# config.afpTool.numberOfAdditionalBc = 1 # for run 348002



# --- create and configure cluster reconstruction tool for each station ---
config.addPrivateTool ("afpTool.clusterToolSt0", "AFP::ClusteringTool")
config.addPrivateTool ("afpTool.clusterToolSt1", "AFP::ClusteringTool")
config.addPrivateTool ("afpTool.clusterToolSt2", "AFP::ClusteringTool")
config.addPrivateTool ("afpTool.clusterToolSt3", "AFP::ClusteringTool")



# --- create and configure track finding tool for each station ---
config.addPrivateTool ("afpTool.trackFindToolSt0", "AFP::TrackFindSimpleClusteringTool")
config.addPrivateTool ("afpTool.trackFindToolSt1", "AFP::TrackFindSimpleClusteringTool")
config.addPrivateTool ("afpTool.trackFindToolSt2", "AFP::TrackFindSimpleClusteringTool")
config.addPrivateTool ("afpTool.trackFindToolSt3", "AFP::TrackFindSimpleClusteringTool")
config.afpTool.trackFindToolSt0.allowedDistanceBetweenClustersInTrack = 1.0  # set distance between tracks
config.afpTool.trackFindToolSt1.allowedDistanceBetweenClustersInTrack = 1.0  # set distance between tracks
config.afpTool.trackFindToolSt2.allowedDistanceBetweenClustersInTrack = 0.8  # set distance between tracks
config.afpTool.trackFindToolSt3.allowedDistanceBetweenClustersInTrack = 0.8  # set distance between tracks



# --- create and configure track reconstruction tool for each station ---
config.addPrivateTool ("afpTool.trackRecoToolSt0", "AFP::TrackRecoLinRegTool")
config.addPrivateTool ("afpTool.trackRecoToolSt1", "AFP::TrackRecoLinRegTool")
config.addPrivateTool ("afpTool.trackRecoToolSt2", "AFP::TrackRecoLinRegTool")
config.addPrivateTool ("afpTool.trackRecoToolSt3", "AFP::TrackRecoLinRegTool")



# --- create and configure proton reconstruction tool for each side ---
### ProtonRecoAnalytical:
config.addPrivateTool ("afpTool.protonRecoToolSide0", "AFP::ProtonRecoAnalytical")
config.addPrivateTool ("afpTool.protonRecoToolSide1", "AFP::ProtonRecoAnalytical")

# Side A
afpTool.protonRecoToolSide0.parametrizationFileName = "parameterization_beta_0.40_b2_2017.txt"

# Side C
afpTool.protonRecoToolSide1.parametrizationFileName = "parameterization_beta_0.40_b1_2017.txt"


# ### ProtonRecoChi2Minuit:
# config.addPrivateTool ("afpTool.protonRecoToolSide0", "AFP::ProtonRecoChi2Minuit")
# config.addPrivateTool ("afpTool.protonRecoToolSide1", "AFP::ProtonRecoChi2Minuit")
#
# # Side A
# config.addPrivateTool ("afpTool.protonRecoToolSide0.transportToolAFP", "AFP::TransportParametrization")
# config.afpTool.protonRecoToolSide0.transportToolAFP.parametrizationFileNameBeam1 = "parameterization_beta_0.40_b1_2017.txt"
# config.afpTool.protonRecoToolSide0.transportToolAFP.parametrizationFileNameBeam2 = "parameterization_beta_0.40_b2_2017.txt"
#
# # Side C
# config.addPrivateTool ("afpTool.protonRecoToolSide1.transportToolAFP", "AFP::TransportParametrization")
# config.afpTool.protonRecoToolSide1.transportToolAFP.parametrizationFileNameBeam1 = "parameterization_beta_0.40_b1_2017.txt"
# config.afpTool.protonRecoToolSide1.transportToolAFP.parametrizationFileNameBeam2 = "parameterization_beta_0.40_b2_2017.txt"


# ### ProtonRecoWithALFA:
# config.addPrivateTool ("afpTool.protonRecoToolSide0", "AFP::ProtonRecoWithALFA")
# config.addPrivateTool ("afpTool.protonRecoToolSide1", "AFP::ProtonRecoWithALFA")
#
# # Side A
# config.addPrivateTool ("afpTool.protonRecoToolSide0.transportToolAFP", "AFP::TransportParametrization")
# config.afpTool.protonRecoToolSide0.transportToolAFP.parametrizationFileNameBeam1 = "param_run348002_AFP_b1.txt"
# config.afpTool.protonRecoToolSide0.transportToolAFP.parametrizationFileNameBeam2 = "param_run348002_AFP_b2.txt"
#
# config.addPrivateTool ("afpTool.protonRecoToolSide0.transportToolALFA", "AFP::TransportParametrization")
# config.afpTool.protonRecoToolSide0.transportToolALFA.parametrizationFileNameBeam1 = "param_run348002_ALFA_b1.txt"
# config.afpTool.protonRecoToolSide0.transportToolALFA.parametrizationFileNameBeam2 = "param_run348002_ALFA_b2.txt"
# config.afpTool.protonRecoToolSide0.transportToolALFA.nearStationPosition = 237.398
# config.afpTool.protonRecoToolSide0.transportToolALFA.farStationPosition = 245.657
#
# # Side C
# config.addPrivateTool ("afpTool.protonRecoToolSide1.transportToolAFP", "AFP::TransportParametrization")
# config.afpTool.protonRecoToolSide1.transportToolAFP.parametrizationFileNameBeam1 = "param_run348002_AFP_b1.txt"
# config.afpTool.protonRecoToolSide1.transportToolAFP.parametrizationFileNameBeam2 = "param_run348002_AFP_b2.txt"
#
# config.addPrivateTool ("afpTool.protonRecoToolSide1.transportToolALFA", "AFP::TransportParametrization")
# config.afpTool.protonRecoToolSide1.transportToolALFA.parametrizationFileNameBeam1 = "param_run348002_ALFA_b1.txt"
# config.afpTool.protonRecoToolSide1.transportToolALFA.parametrizationFileNameBeam2 = "param_run348002_ALFA_b2.txt"
# config.afpTool.protonRecoToolSide1.transportToolALFA.nearStationPosition = 237.398
# config.afpTool.protonRecoToolSide1.transportToolALFA.farStationPosition = 245.656


# &&&&& END OPTIONAL CONFIGURATION &&&&&

# Print configuration
# print('\n%s\n') % (config.__str__())


job.algsAdd( config )

driver = ROOT.EL.DirectDriver()
driver.submit( job, options.submission_dir )

