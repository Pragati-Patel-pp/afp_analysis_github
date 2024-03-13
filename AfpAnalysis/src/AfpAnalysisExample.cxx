
#include <AfpAnalysis/AfpAnalysisExample.h>

#include <xAODRootAccess/Init.h>
#include <xAODRootAccess/TEvent.h>
#include <xAODEventInfo/EventInfo.h>
#include <xAODForward/AFPSiHitContainer.h>
#include <xAODForward/AFPSiHit.h>
#include <xAODJet/JetContainer.h>
#include <AfpAnalysisTools/AfpAnalysisTool.h>
#include <PATInterfaces/SystematicsUtil.h>
#include <xAODCaloEvent/CaloClusterContainer.h>
#include <TH1.h>
#include <utility>
#include <iostream>
#include <cmath>
#include <fstream>

AfpAnalysisExample ::AfpAnalysisExample(const std::string &name, ISvcLocator *pSvcLocator) : EL::AnaAlgorithm(name, pSvcLocator),
                                                                                             m_afpTool("AFP::AfpAnalysisTool/afpTool", this),
                                                                                             m_trigConfigTool("TrigConf::xAODConfigTool/xAODConfigTool"),
                                                                                             m_trigDecisionTool("Trig::TrigDecisionTool/TrigDecisionTool"),
                                                                                             //jet_file("/eos/user/p/prpatel/afp_analysis/run/out_aod.txt")
                                                                                             jet_file("/eos/user/p/prpatel/combined_file.txt")
                                                                                             //current_pos(0)
                                                                                             // aod_event_file("/eos/user/p/prpatel/afp_analysis/run/aod_event.txt")
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



  ANA_CHECK(book(TH2F("xi_vs_x_pos_A_loose", "xi_vs_x_pos_A_loose", 100, -20, 0, 100, 0, 0.2)));
  ANA_CHECK(book(TH2F("xi_vs_x_pos_C_loose", "xi_vs_x_pos_C_loose", 100, -20, 0, 100, 0, 0.2)));
  
  ANA_CHECK(book(TH2F("xi_cal_vs_x_pos_A", "xi_cal_vs_x_pos_A", 100, -20, 0, 100, 0, 0.2)));
  ANA_CHECK(book(TH2F("xi_cal_vs_x_pos_C", "xi_cal_vs_x_pos_C", 100, -20, 0, 100, 0, 0.2)));

  /*
  ANA_CHECK(book(TH1F("xi_cal_signal", "xi_cal_signal", 100, 0, 0.2)));
  ANA_CHECK(book(TH1F("xi_cal_background", "xi_cal_background", 100, 0, 0.2)));

  ANA_CHECK(book(TH1F("E_dijet/E_cal", "E_dijet/E_cal", 100, 0, 1.0)));
  ANA_CHECK(book(TH2F("E_lead_jet+E_sublead_jet_vs_x_pos_A", "E_lead_jet+E_sublead_jet vs x_pos_A", 100, -20, 0, 100, 0, -1)));
  ANA_CHECK(book(TH2F("E_lead_jet+E_sublead_jet_vs_x_pos_C", "E_lead_jet+E_sublead_jet vs x_pos_C", 100, -20, 0, 100, 0, -1)));

  ANA_CHECK(book(TH1F("pt_lead_jet_xi_cal<0.1", "pt_lead_jet_xi_cal<0.1", 100, 0, -1)));

  ANA_CHECK(book(TH1F("eta_lead_jet", "eta_lead_jet", 100, -5, 5)));
  ANA_CHECK(book(TH1F("eta_sublead_jet", "eta_sublead_jet", 100, -5, 5)));

  ANA_CHECK(book(TH1F("phi_lead_jet", "phi_lead_jet", 100, -5, 5)));
  ANA_CHECK(book(TH1F("phi_sublead_jet", "phi_sublead_jet", 100, -5, 5)));

  */
  xAOD::TEvent *event = wk()->xaodEvent();
  TFile *file = wk()->getOutputFile("skim");
  ANA_CHECK(event->writeTo(file));

  //check if the file is open
  if (jet_file.is_open())
  {
    std::cout << "File is open" << std::endl;
  }
  else
  {
    std::cout << "File is not open" << std::endl;
  }

  
  return StatusCode::SUCCESS;
}

StatusCode AfpAnalysisExample ::execute()
{
  xAOD::TEvent *event = wk()->xaodEvent();

  // const xAOD::JetContainer* AntiKt4EMPFlowJets = 0;
  // ANA_CHECK(event->retrieve( AntiKt4EMPFlowJets, "AntiKt4EMPFlowJets" ) );

  const xAOD::EventInfo *eventInfo = 0;
  ANA_CHECK(event->retrieve(eventInfo, "EventInfo"));

  const xAOD::CaloClusterContainer *CaloCalTopoClusters = 0;
  ANA_CHECK(event->retrieve(CaloCalTopoClusters, "CaloCalTopoClusters"));

  long event_number = eventInfo->eventNumber();


  std::string line;
  jet_file.seekg(current_pos); 

  // read only current line
  std::getline(jet_file, line);
  std::istringstream iss(line);

  int event_number_jet;
  float leading_jet_px, leading_jet_py, leading_jet_pz, leading_jet_e, subleading_jet_px, subleading_jet_py, subleading_jet_pz, subleading_jet_e;
  iss >> event_number_jet >> leading_jet_px >> leading_jet_py >> leading_jet_pz >> leading_jet_e >> subleading_jet_px >> subleading_jet_py >> subleading_jet_pz >> subleading_jet_e;

  //std::cout<<event_number_jet<<"\t"<<event_number<<std::endl;

  if (event_number_jet == event_number)
  {
    //std::cout << "Event number: " << event_number_jet << std::endl;
    int clusters_id = 0;
    std::map<int, float> myMap;
    // set the current position to the next line else keep it same
    current_pos = jet_file.tellg();

    int nclusters[4][4][2]; // 4 stations, 4 layers, 2 values

    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        nclusters[i][j][0] = 0;
        nclusters[i][j][1] = 0;
      }
    }

    // calculate the number of clusters in each station and layer and the x position of each cluster in the event
    for (auto cluster : *(m_afpTool->clusters()))
    {
      // std::cout << "Station: " << cluster->stationID() << " Layer: " << cluster->pixelLayerID() << " x: " << cluster->xLocal() << " clusterID: " << clusters_id << std::endl;
      int s = cluster->stationID();
      int l = cluster->pixelLayerID();
      float x = cluster->xLocal();
      nclusters[s][l][0]++;
      clusters_id++;
      nclusters[s][l][1] = clusters_id;
      myMap[clusters_id] = x;
    }
    // check if the clusters are being read correctly

    // std::cout << "Number of clusters in station 0: " << nclusters_station_0 << std::endl;
    //hist("cluster_multiplicity")->Fill(nclusters[2][0][0] + nclusters[2][1][0] + nclusters[2][2][0] + nclusters[2][3][0] + nclusters[3][0][0] + nclusters[3][1][0] + nclusters[3][2][0] + nclusters[3][3][0]);
    TLorentzVector leadingJetVec = TLorentzVector(leading_jet_px, leading_jet_py, leading_jet_pz, leading_jet_e);
    TLorentzVector subleadingJetVec = TLorentzVector(subleading_jet_px, subleading_jet_py, subleading_jet_pz, subleading_jet_e);
    TLorentzVector sum = leadingJetVec + subleadingJetVec;

    // count the number of layers in each station with exactly 1 cluster and store it in a map
    int number_of_layers_with_1_cluster[4] = {0, 0, 0, 0};
    for (int i = 0; i < 4; i++)
    {
      int count = 0;
      for (int j = 0; j < 4; j++)
      {
        if (nclusters[i][j][0] == 1)
        {
          count++;
        }
      }
      number_of_layers_with_1_cluster[i] = count;
    }

    if (number_of_layers_with_1_cluster[0] >= 3 && number_of_layers_with_1_cluster[1] >= 3)
    {
      // std::cout<<"Event number: "<<event_number_jet<<"\t A loose"<<std::endl;
      float xi = sum.M() * std::exp(sum.Rapidity()) / (13.6 * 1e6);
      int id = nclusters[0][0][1];
      float x_pos_A = myMap[id];
      // std::cout<<"xi: "<<xi<<"\t x_pos_A: "<<x_pos_A<<" id: "<<id<<"x_pos_A: "<<x_pos_A<<"xi: "<<xi<<std::endl;
      hist("xi_vs_x_pos_A_loose")->Fill(x_pos_A, xi);

      float xi_cal_A = 0;

      for (auto cl : *CaloCalTopoClusters)
      {
        xi_cal_A += cl->pt() * exp(cl->eta()) / (13.6 * 1e6);
      }
      hist("xi_cal_vs_x_pos_A")->Fill(x_pos_A, xi_cal_A);
    }

    if (number_of_layers_with_1_cluster[2] >= 3 && number_of_layers_with_1_cluster[3] >= 3)
    {
      // std::cout<<"Event number: "<<event_number_jet<<"\t C loose"<<std::endl;
      float xi = sum.M() * std::exp(-sum.Rapidity()) / (13.6 * 1e6);
      int id = nclusters[2][0][1];
      float x_pos_C = myMap[id];
      // std::cout<<"xi: "<<xi<<"\t x_pos_A: "<<x_pos_A<<" id: "<<id<<"x_pos_A: "<<x_pos_A<<"xi: "<<xi<<std::endl;
      hist("xi_vs_x_pos_C_loose")->Fill(x_pos_C, xi);

      float xi_cal_C = 0;

      for (auto cl : *CaloCalTopoClusters)
      {
        xi_cal_C += cl->pt() * exp(-cl->eta()) / (13.6 * 1e6);
      }
      hist("xi_cal_vs_x_pos_C")->Fill(x_pos_C, xi_cal_C);
    }


  } // if(event_number_jet == event_number) ends here  

  return StatusCode::SUCCESS;
}

StatusCode AfpAnalysisExample ::finalize()
{

  xAOD::TEvent *event = wk()->xaodEvent();
  TFile *file = wk()->getOutputFile("skim");
  ANA_CHECK(event->finishWritingTo(file));
  
  if(jet_file.is_open())
  {
    jet_file.close();
    std::cout << "File is closed" << std::endl;
  }

  return StatusCode::SUCCESS;
}

// vim: expandtab tabstop=8 shiftwidth=2 softtabstop=2
