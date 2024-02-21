#pragma once

#include "AfpAnalysisTools/IAfpAnalysisTool.h"
#include <TrigConfInterfaces/ITrigConfigTool.h>
#include <TrigDecisionTool/TrigDecisionTool.h>

#ifdef XAOD_STANDALONE
  #include "EventLoop/Job.h"
  #include "EventLoop/StatusCode.h"
  #include "EventLoop/Worker.h"
#endif  

#include "AsgTools/AnaToolHandle.h"
#include "AsgTools/MessageCheckAsgTools.h" // ASG status code check
#include "AnaAlgorithm/AnaAlgorithm.h"
#include "JetCalibTools/IJetCalibrationTool.h"

class AfpAnalysisExample : public EL::AnaAlgorithm
{
  std::ofstream my_output_file;

  public:
   
    AfpAnalysisExample (const std::string& name, ISvcLocator* pSvcLocator);

    virtual StatusCode initialize ();
    virtual StatusCode execute ();
    virtual StatusCode finalize ();
    std::string m_Trigger{};
    
    


  private:

    /// List of systematics (optional)
    // std::vector<CP::SystematicSet> m_systematicsList;

    /// AfpAnalysisToolbox handle
     asg::AnaToolHandle<AFP::IAfpAnalysisTool> m_afpTool; //!
     asg::AnaToolHandle<TrigConf::ITrigConfigTool> m_trigConfigTool; // !
     asg::AnaToolHandle<Trig::TrigDecisionTool> m_trigDecisionTool; // !
     asg::AnaToolHandle<IJetCalibrationTool> m_jetCalibration; // !
     // read a file
     //std::ifstream jet_file;
     //std::streampos current_pos = 0;

     //create a file
     // std::ofstream jet_event_file;
     // std::string jet_event_file_name = "all_aod_event.txt";


     


};

// vim: expandtab tabstop=8 shiftwidth=2 softtabstop=2
