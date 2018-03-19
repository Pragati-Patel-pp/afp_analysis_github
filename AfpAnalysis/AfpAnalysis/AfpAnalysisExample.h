#pragma once

#include "AsgTools/AnaToolHandle.h"

#include <AnaAlgorithm/AnaAlgorithm.h>
#include <AfpAnalysisTools/IAfpAnalysisTool.h>

class AfpAnalysisExample : public EL::AnaAlgorithm
{
  
  public:
   
    AfpAnalysisExample (const std::string& name, ISvcLocator* pSvcLocator);

    virtual StatusCode initialize ();
    virtual StatusCode execute ();
    virtual StatusCode finalize ();

  private:

    asg::AnaToolHandle<AFP::IAfpAnalysisTool> m_afpTool; //!

};

// vim: expandtab tabstop=8 shiftwidth=2 softtabstop=2
