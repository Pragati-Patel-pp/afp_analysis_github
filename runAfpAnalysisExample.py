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
inputFilePath = "/eos/user/r/rafal/AFP/data17_13TeV.00331975.physics_Main.merge.AOD.r10203_p3399/"
ROOT.SH.ScanDir().filePattern( '*' ).scan( sh, inputFilePath )

job = ROOT.EL.Job()
job.sampleHandler( sh )
job.options().setDouble( ROOT.EL.Job.optMaxEvents, 500 )

from AnaAlgorithm.AnaAlgorithmConfig import AnaAlgorithmConfig
config = AnaAlgorithmConfig( 'AfpAnalysisExample' )
job.algsAdd( config )

driver = ROOT.EL.DirectDriver()
driver.submit( job, options.submission_dir )

