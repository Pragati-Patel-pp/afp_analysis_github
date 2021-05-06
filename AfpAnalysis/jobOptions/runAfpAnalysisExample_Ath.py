
# it's important to have "POOLAccess" here. With "AthenaAccess" or others, the EventContext won't be filled properly
jps.AthenaCommonFlags.AccessMode = "POOLAccess" 

# overwrite with --evtMax
jps.AthenaCommonFlags.EvtMax=100


# overwrite with --filesInput
# athena AfpAnalysis/runAfpAnalysisExample_Ath.py --filesInput="data17_13TeV.00338480.physics_Main.daq.RAW._lb0275._SFO-7._0007.AOD.root"
jps.AthenaCommonFlags.FilesInput = ["/eos/atlas/atlascerngroupdisk/det-afp/AnalysisData/Tutorial/data17_13TeV.00331975.physics_Main.merge.AOD.r10203_p3399/AOD.12866891._003111.pool.root.1"]


from IOVDbSvc.CondDB import conddb

# without this, we would get an error:
# IOVDbSvc             INFO Global tag: CONDBR2-BLKPA-2017-15 set from input file
# IOVDbSvc            FATAL Likely incorrect conditions DB configuration! Attached to database instance  'DBInstance':'OFLP200' but global tag begins CONDBR2
# IOVDbSvc            FATAL See Atlas/CoolTroubles wiki for details, or set IOVDbSvc.DBInstance="" to disable check
conddb.iovdbsvc.DBInstance=""


schema = "<db>sqlite://;schema=Example.db;dbname=CONDBR2</db>"
locFolder = "/FWD/AFP/LocalAlignment"
locTag="<tag>AFPAlignLoc-00</tag>"
conddb.addFolder("", schema+locFolder+locTag, className='CondAttrListCollection')

globFolder = "/FWD/AFP/GlobalAlignment"
globTag="<tag>AFPAlignGlob-00</tag>"
conddb.addFolder("", schema+globFolder+globTag, className='CondAttrListCollection')


algo = CfgMgr.AfpAnalysisExample("AfpAnalysisExample", OutputLevel=INFO)


athAlgSeq += algo


