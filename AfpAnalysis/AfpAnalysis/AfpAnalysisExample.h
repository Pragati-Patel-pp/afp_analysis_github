#pragma once

#include "AsgTools/AnaToolHandle.h"

#include <EventLoop/Algorithm.h>
#include <AfpTools/AfpTool.h>

class AfpAnalysisExample : public EL::Algorithm
{
  
  public:
   
    AfpAnalysisExample ();

    virtual EL::StatusCode setupJob (EL::Job& job);
    virtual EL::StatusCode fileExecute ();
    virtual EL::StatusCode histInitialize ();
    virtual EL::StatusCode changeInput (bool firstFile);
    virtual EL::StatusCode initialize ();
    virtual EL::StatusCode execute ();
    virtual EL::StatusCode postExecute ();
    virtual EL::StatusCode finalize ();
    virtual EL::StatusCode histFinalize ();

  private:
    static constexpr int nstations = 4;
    static constexpr int nlayers = 4;

    asg::AnaToolHandle<AfpTools::IAfpTool> m_AfpTool; //!

    ClassDef(AfpAnalysisExample, 1);
};

// vim: expandtab tabstop=8 shiftwidth=2 softtabstop=2
