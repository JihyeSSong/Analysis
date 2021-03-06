#ifndef AliXiStarPbPb_H
#define AliXiStarPbPb_H
//
// Class AliXiStarPbPb
//
// AliXiStarPbPb
// author:
//        Dhevan Gangadharan (dhevan.raja.gangadharan@cern.ch)
//



class TH1F;
class TH1D;
class TH2D;
class TH3D;
class TProfile;
class TTree;

class AliESDEvent;
class AliESDtrackCuts;
class AliESDpid;

#include "AliAnalysisTask.h"
#include "AliAnalysisTaskSE.h"
#include "AliAODPid.h"
#include "AliESDpid.h"
#include "AliXiStarPbPbEventCollection.h"



class AliXiStarPbPb : public AliAnalysisTaskSE {
public:
    
    AliXiStarPbPb();
    AliXiStarPbPb(const char *name, Bool_t AODdecision, Bool_t MCdecision, Int_t CutListOption=0);

    virtual ~AliXiStarPbPb();
    AliXiStarPbPb(const AliXiStarPbPb &obj);
    AliXiStarPbPb &operator=(const AliXiStarPbPb &obj);
    
private:
    
    virtual void   UserCreateOutputObjects();
    virtual void   Exec(Option_t *option);
    virtual void   Terminate(Option_t *);
    
    
    void XiStarInit();// initialization of fixed values
    Double_t LinearPropagateToDCA(AliESDtrack*, AliESDtrack*, Double_t);// for linear propagation
    Double_t Det(Double_t, Double_t, Double_t, Double_t) const;// for linear propagation
    Double_t Det(Double_t, Double_t, Double_t, Double_t, Double_t, Double_t, Double_t, Double_t, Double_t) const;// for linear propagation
    Double_t ComputeCentrality();
    Double_t ApplyCentralityPatchPbPb2011();
    Double_t ApplyCentFlatPbPb2011();
    
    void SetCentralityEstimator (TString lCentralityEstimator = "V0M" ) { fCentralityEstimator = lCentralityEstimator; }
    
    
    
    
    enum {
        kNbinsM              = 200, // mult bins for certain histograms //300
        kXiStarCode          = 3324,// Xi(1530)^0 MC code
        kXiCode              = 3312,// Xi- MC code
        kLambdaCode          = 3122,// Lambda MC code
        kProtonCode          = 2212,// Proton+ MC code
        kPionCode            = 211,// Pion+ MC code
        kNCutVariations      = 21,// number of cut variations // 13
        kNCuts               = 13// number of cut types //15
    };
    
    const char* fname;// name of class
   // AliInputEventHandler *fEventHandler;                              //  for ESDs or AODs
    AliESDEvent            *fESD; //!    // ESD object
    TList                  *fOutputList; //! Compact Output list
    TList                  *fOutputListTree; //! Compact Output list
    AliESDtrackCuts        *fTrackCut; //! ESD track cuts
    AliPIDResponse         *fPIDResponse; //! PID object
    AliCentrality          *fCentrality; //! Centrality object
    TTree                  *fOutputTree; //*fTreeXi, *fTreePi, *fTreeEvent, *fTreeMC; //! Output Tree
    
    AliXiStarPbPbEventCollection ***fEC; //! The event collection
    AliXiStarPbPbEventStruct *fEvt; //! The current event type
    AliXiStarPbPbTrackStruct *fTempStruct; //! A temporary track storage.  Eventually put into fEvt
    
    //
    Int_t fZvertexBins;// number of Z-vertex bins for event-mixing
    Int_t fEventsToMix;// number of maximum events to mix
    Int_t fMultBins;// number of multiplicity bins for event-mixing
    Int_t fMultLimits[11+1];// the multiplicity edges of the mult bins
    Bool_t fMCcase;// switch for MC data or real data
    Bool_t fAODcase;// switch for AODs or ESDs
    Int_t fEventCounter;// The event counter
    ULong64_t fEventNumber; // calcuate event number

    TString fCentralityEstimator; // Centrality Estimator string value (default V0A in pPb V0M in PbPb)
    // cut list data members
    Float_t fMaxDecayLength;// max decay length
    Float_t fMassWindow;// Mass window of acceptance for Lambda and Xi candidates
    
    Double_t fCovMatrix[21];// Covarience matrix of track
    Double_t fTrueMassPr, fTrueMassPi, fTrueMassK, fTrueMassLam, fTrueMassXi;// The PDG mass values
    Bool_t IsTPC  (AliESDtrack *track);

  
    
    
    //=================================================================================//
    //generated Histograms//
    //=================================================================================//
    
    struct St_CutType {
        TH3F *fXi; //!
        TH3F *fXibar; //!
        //
        TH3F *fXiMinusPiPlus; //!
        TH3F *fXiMinusPiMinus; //!
        TH3F *fXiPlusPiPlus; //!
        TH3F *fXiPlusPiMinus; //!
        
        TH3F *fXiMinusPiPlusbkg; //!
        TH3F *fXiMinusPiMinusbkg; //!
        TH3F *fXiPlusPiPlusbkg; //!
        TH3F *fXiPlusPiMinusbkg; //!
        //
        TH3F *fMCrecXi; //!
        TH3F *fMCrecXibar; //!
        
        TH3F *fMCrecXiMinusPiPlus; //!
        TH3F *fMCrecXiPlusPiMinus; //!
       
    };
    struct St_CutType CutVar[kNCutVariations]; //!
    
    //=================================================================================//
    //generated Tree variables//
    //=================================================================================//
    struct St_CutTypeTree {
        TTree *fTTree;
        TTree *fTTreeEMBG;

    };
    struct St_CutTypeTree CutVarTree[kNCutVariations]; //!
    
    Float_t fTreeMass;
    Float_t fTreePt;
    Float_t fTreeCent;
    Float_t fTreeChargeXi;
    Float_t fTreeChargePi;
    
    Float_t fTreeMassEMBG;
    Float_t fTreePtEMBG;
    Float_t fTreeCentEMBG;
    Float_t fTreeChargeXiEMBG;
    Float_t fTreeChargePiEMBG;


    
    
    AliESDtrack* fESDTrack4; //! esdtrack for XiStar's daughter pion
    AliESDtrack* fXiTrack; //! esdtrack for XiStar's daughter Xi
    
    Int_t fCutList;// Cut List option (mean values or systematic variations)
    
    Float_t fDecayParameters[kNCuts];// array of reconstruction kinematics
    Float_t fCutValues[kNCutVariations][kNCuts];// array of reconstruction kinematics
    
    ClassDef(AliXiStarPbPb, 1); 
};

#endif
