
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
  // ANA_CHECK(m_triggerMenuMetaDataTool.initialize());

  // ANA_CHECK(book(TH2F("xi_vs_x_pos_A_loose", "xi vs x_pos_A_loose", 100, -20, 0, 100, 0, 1)));
  // ANA_CHECK(book(TH2F("xi_vs_x_pos_C_loose", "xi vs x_pos_C_loose", 100, -20, 0, 100, 0, 1)));
  // ANA_CHECK(book(TH2F("xi_vs_x_pos_A_tight", "xi vs x_pos_A_tight", 100, -20, 0, 100, 0, 1)));
  // ANA_CHECK(book(TH2F("xi_vs_x_pos_C_tight", "xi vs x_pos_C_tight", 100, -20, 0, 100, 0, 1)));

  // ANA_CHECK(book(TH1F("cluster_multiplicity", "cluster_multiplicity", 20,-0.5,19.5)));

  xAOD::TEvent *event = wk()->xaodEvent();
  TFile *file = wk()->getOutputFile("skim");
  ANA_CHECK(event->writeTo(file));

  TFile *file2 = wk()->getOutputFile("txt");
  ANA_CHECK(event->writeTo(file2));


 

  return StatusCode::SUCCESS;
}

StatusCode AfpAnalysisExample ::execute()
{
  xAOD::TEvent *event = wk()->xaodEvent();

  // const xAOD::JetContainer* AntiKt4EMPFlowJets = 0;
  // ANA_CHECK(event->retrieve( AntiKt4EMPFlowJets, "AntiKt4EMPFlowJets" ) );

  const xAOD::EventInfo *eventInfo = 0;
  ANA_CHECK(event->retrieve(eventInfo, "EventInfo"));

  // write the event number to the 

  return StatusCode::SUCCESS;
}

StatusCode AfpAnalysisExample ::finalize()
{

  xAOD::TEvent *event = wk()->xaodEvent();
  TFile *file = wk()->getOutputFile("skim");
  ANA_CHECK(event->finishWritingTo(file));

  TFile *file2 = wk()->getOutputFile("txt");
  ANA_CHECK(event->finishWritingTo(file2));
  

  return StatusCode::SUCCESS;
}

// vim: expandtab tabstop=8 shiftwidth=2 softtabstop=2
