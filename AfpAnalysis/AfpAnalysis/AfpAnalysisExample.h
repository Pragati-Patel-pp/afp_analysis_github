#pragma once

#include "AfpAnalysisTools/IAfpAnalysisTool.h"

#ifdef XAOD_STANDALONE
  #include "EventLoop/Job.h"
  #include "EventLoop/StatusCode.h"
  #include "EventLoop/Worker.h"
#endif  

#include "AsgTools/AnaToolHandle.h"
#include "AsgTools/MessageCheckAsgTools.h" // ASG status code check
#include "AnaAlgorithm/AnaAlgorithm.h"

class AfpAnalysisExample : public EL::AnaAlgorithm
{
  
  public:
   
    AfpAnalysisExample (const std::string& name, ISvcLocator* pSvcLocator);

    virtual StatusCode initialize ();
    virtual StatusCode execute ();
    virtual StatusCode finalize ();

  private:

    /// List of systematics (optional)
    // std::vector<CP::SystematicSet> m_systematicsList;

    /// AfpAnalysisToolbox handle
    asg::AnaToolHandle<AFP::IAfpAnalysisTool> m_afpTool; //!

};

// vim: expandtab tabstop=8 shiftwidth=2 softtabstop=2
