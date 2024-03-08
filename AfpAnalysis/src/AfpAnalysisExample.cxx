
#include <AfpAnalysis/AfpAnalysisExample.h>

#include <xAODRootAccess/Init.h>
#include <xAODRootAccess/TEvent.h>
#include <xAODEventInfo/EventInfo.h>
#include <xAODForward/AFPSiHitContainer.h>
#include <xAODForward/AFPSiHit.h>
#include <xAODJet/JetContainer.h>
#include <AfpAnalysisTools/AfpAnalysisTool.h>
#include <PATInterfaces/SystematicsUtil.h>
#include <TH1.h>
#include <utility>
#include <iostream>
#include <cmath>
#include <fstream>
#include <TTree.h>
#include <TSystem.h>
AfpAnalysisExample ::AfpAnalysisExample(const std::string &name, ISvcLocator *pSvcLocator) : EL::AnaAlgorithm(name, pSvcLocator),
                                                                                             m_afpTool("AFP::AfpAnalysisTool/afpTool", this),
                                                                                             m_trigConfigTool("TrigConf::xAODConfigTool/xAODConfigTool"),
                                                                                             m_trigDecisionTool("Trig::TrigDecisionTool/TrigDecisionTool")
                                                                                          
                                                                                             //jet_event_file("/eos/user/p/prpatel/afp_analysis/run/all_aod_event.txt", std::ios::app)
// current_pos(0)
//  aod_event_file("/eos/user/p/prpatel/afp_analysis/run/aod_event.txt")
{
  m_afpTool.declarePropertyFor(this, "afpTool", "Tool providing AFP information");
  m_afpTool.setTypeAndName("AFP::AfpAnalysisTool/afpTool");
  declareProperty("Trigger", m_Trigger = "HLT_j20_L1AFP_A_OR_C", "Trigger");
}

StatusCode AfpAnalysisExample ::initialize()
{
  ANA_CHECK(m_afpTool.retrieve());

  ANA_MSG_INFO(m_afpTool->configInfo());

  // Initialize the TrigDecisionTool
  ANA_CHECK(m_trigConfigTool.initialize());
  ANA_CHECK(m_trigDecisionTool.setProperty("ConfigTool", m_trigConfigTool.getHandle()));
  ANA_CHECK(m_trigDecisionTool.setProperty("TrigDecisionKey", "xTrigDecision"));
  ANA_CHECK(m_trigDecisionTool.initialize());
  //create an object to store the event number
  //ANA_CHECK(book(TH1F("event_number", "event_number", 1, 0, -1)));

  xAOD::TEvent *event = wk()->xaodEvent();
  TFile *file = wk()->getOutputFile("skim");
  ANA_CHECK(event->writeTo(file));


  std::string file_name = wk()->inputFileName();
  //std::cout << "file_name = " << file_name << std::endl;

  mytree = new TTree("TreeHits","TreeHits");
  wk()->addOutput(mytree);

  //mytree->Branch("ev",&ev,"ev/I");
  //mytree->Branch(file_name.c_str(), &ev, (file_name + "/I").c_str() );


  return StatusCode::SUCCESS;
}

StatusCode AfpAnalysisExample ::execute()
{
  xAOD::TEvent *event = wk()->xaodEvent();
  std::string currentFileName = wk()->inputFileName();


if (currentFileName != m_lastFileName) {
    std::cout << "Processing a new file: " << currentFileName << std::endl;

    m_lastFileName = currentFileName;
}




if (!mytree->GetBranch(currentFileName.c_str())) {
    mytree->Branch(currentFileName.c_str(), &ev, (currentFileName + "/I").c_str());
}
  const xAOD::EventInfo *eventInfo = 0;
  ANA_CHECK(event->retrieve(eventInfo, "EventInfo"));

  //print luminosity block number only if it changes
  if (eventInfo->lumiBlock() != m_lumiBlock) {
    m_lumiBlock = eventInfo->lumiBlock();
    std::cout << "Processing luminosity block " << m_lumiBlock << std::endl;
  }

  ev = eventInfo->eventNumber();
  mytree->Fill();

  return StatusCode::SUCCESS;
}

StatusCode AfpAnalysisExample ::finalize()
{

  xAOD::TEvent *event = wk()->xaodEvent();
  TFile *file = wk()->getOutputFile("skim");
  ANA_CHECK(event->finishWritingTo(file));

  //event_number_file.close();


  return StatusCode::SUCCESS;
}

// vim: expandtab tabstop=8 shiftwidth=2 softtabstop=2
