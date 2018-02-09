#pragma once

#include "AsgTools/AnaToolHandle.h"

#include <AnaAlgorithm/AnaAlgorithm.h>
#include <AfpAnalysisTools/AfpTool.h>

class AfpAnalysisExample : public EL::AnaAlgorithm
{
  
  public:
   
    AfpAnalysisExample (const std::string& name, ISvcLocator* pSvcLocator);

    virtual StatusCode initialize ();
    virtual StatusCode execute ();
    virtual StatusCode finalize ();

  private:

    asg::AnaToolHandle<AFP::IAfpTool> m_AfpTool; //!

    ClassDef(AfpAnalysisExample, 1);
};

// vim: expandtab tabstop=8 shiftwidth=2 softtabstop=2
