#ifndef ALIXISTAR_H
#define ALIXISTAR_H
//
// Class AliXiStar
//
// AliXiStar
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
class AliAODEvent;
class AliESDtrackCuts;
class AliESDpid;

#include "AliAnalysisTask.h"
#include "AliAnalysisTaskSE.h"
#include "AliAODPid.h"
#include "AliESDpid.h"
#include "AliXiStarEventCollection.h"

class AliXiStar : public AliAnalysisTaskSE {
public:
    
    AliXiStar();
    AliXiStar(const char *name, Bool_t AODdecision, Bool_t MCdecision, Int_t CutListOption=0);
    virtual ~AliXiStar();
    AliXiStar(const AliXiStar &obj);
    AliXiStar &operator=(const AliXiStar &obj);
    
private:
    
    virtual void   UserCreateOutputObjects();
    virtual void   Exec(Option_t *option);
    virtual void   Terminate(Option_t *);
    
    
    void XiStarInit();// initialization of fixed values
    Double_t LinearPropagateToDCA(AliESDtrack*, AliESDtrack*, Double_t);// for linear propagation
    Double_t Det(Double_t, Double_t, Double_t, Double_t) const;// for linear propagation
    Double_t Det(Double_t, Double_t, Double_t, Double_t, Double_t, Double_t, Double_t, Double_t, Double_t) const;// for linear propagation
    //ULong64_t GetMCEventNumber();
    
    void SetCentralityEstimator (TString lCentralityEstimator = "V0A" ) { fCentralityEstimator = lCentralityEstimator; }
    
    
    
    
    enum {
        kNbinsM              = 300, // mult bins for certain histograms
        kXiStarCode          = 3324,// Xi(1530)^0 MC code
        kXiCode              = 3312,// Xi- MC code
        kLambdaCode          = 3122,// Lambda MC code
        kProtonCode          = 2212,// Proton+ MC code
        kPionCode            = 211,// Pion+ MC code
        kNCutVariations      = 13,// number of cut variations
        kNCuts               = 15 // number of cut types
    };
    
    const char* fname;// name of class
    AliAODEvent            *fAOD; //!    // AOD object
    AliESDEvent            *fESD; //!    // ESD object
    //AliMCEvent             *mcEvent;
    //AliStack               *mcstack;
    TList                  *fOutputList; //! Compact Output list
    AliESDtrackCuts        *fTrackCut; //! ESD track cuts
    AliPIDResponse         *fPIDResponse; //! PID object
    AliCentrality          *fCentrality; //! Centrality object
    TTree                  *fTreeXi, *fTreePi, *fTreeEvent, *fTreeMC; //! Output Tree
    
    AliXiStarEventCollection ***fEC; //! The event collection
    AliXiStarEventStruct *fEvt; //! The current event type
    AliXiStarTrackStruct *fTempStruct; //! A temporary track storage.  Eventually put into fEvt
    
    //
    
    Int_t fZvertexBins;// number of Z-vertex bins for event-mixing
    Int_t fEventsToMix;// number of maximum events to mix
    Int_t fMultBins;// number of multiplicity bins for event-mixing
    Int_t fMultLimits[11+1];// the multiplicity edges of the mult bins
    Bool_t fMCcase;// switch for MC data or real data
    Bool_t fAODcase;// switch for AODs or ESDs
    Int_t fEventCounter;// The event counter
    ULong64_t fEventNumber; // calcuate event number
    ULong64_t fEventNumberMC;

    TString fCentralityEstimator; // Centrality Estimator string value (V0M for PbPb, V0A for pPb)
    
    
    // cut list data members
    Float_t fMaxDecayLength;// max decay length
    Float_t fMassWindow;// Mass window of acceptance for Lambda and Xi candidates
    
    Double_t fCovMatrix[21];// Covarience matrix of track
    Double_t fTrueMassPr, fTrueMassPi, fTrueMassK, fTrueMassLam, fTrueMassXi;// The PDG mass values
    Bool_t IsTPC  (AliESDtrack *track);
    //=================================================================================//
    // Variables for Tree, Casacades
    //=================================================================================//
    //--Tree Variables for cut study--//
    Float_t fTreeVariableMult;
    Float_t fTreeVariablePrimaryVz;
    Float_t fTreeVariableXiMass;
    Float_t fTreeVariableXiPt;
    Float_t fTreeVariableY;
    Float_t fTreeVariableDecayXYxi;
    Float_t fTreeVariableDecayXYlambda;
    
    Float_t fTreeVariableNclusp;
    Float_t fTreeVariableNcluspi1;
    Float_t fTreeVariableNcluspi2;
    
    Float_t fTreeVariablePVdcaP;
    Float_t fTreeVariablePVdcaPi1;
    Float_t fTreeVariablePVdcaPi2;

    Float_t fTreeVariablePVdcaLambda;
    Float_t fTreeVariableDCApip;
    Float_t fTreeVariableDCApilambda;
    Float_t fTreevariableCosPAlambda;
    Float_t fTreevariableCosPAxi;
    
    //--Tree Variables for centrality--//
    Float_t fTreeVariableCentrality;
    Float_t fTreeVariableCentralityV0M;
    Float_t   fTreeVariableCentralityV0A;
    
    //--Tree Variables for TPC PID--//
    Float_t fTreeVariableTPCPIDp;
    Float_t fTreeVariableTPCPIDpi1;
    Float_t fTreeVariableTPCPIDpi2;
    Float_t fTreeVariableTPCPIDpi3;
    
    //-- for matching Xi and Pion events--//
    Int_t  fTreeVariablemyTracks;
    Int_t  fTreeVariablexiTracks;
    
    
    //--Tree Variables for Xi tracks--//
    Float_t fTreeVariableXiTrackPx;
    Float_t fTreeVariableXiTrackPy;
    Float_t fTreeVariableXiTrackPz;
    Float_t fTreeVariableXiTrackQ;
    Float_t fTreeVariableXiTrackpID;
    Float_t fTreeVariableXiTracknID;
    Float_t fTreeVariableXiTrackbID;
    
    
   //--Tree Variables for 3rd Pion tracks--//
    Float_t fTreeVariableESDTrackX;
    Float_t fTreeVariableESDTrackY;
    Float_t fTreeVariableESDTrackZ;
    Float_t fTreeVariableESDTrackPx;
    Float_t fTreeVariableESDTrackPy;
    Float_t fTreeVariableESDTrackPz;
    Float_t fTreeVariableESDTrackQ;
    Float_t fTreeVariableESDTrackID;
    Float_t fTreeVariavleESDStatus;
    Float_t fTreeVariableESDtrackDCAZ;
    Float_t fTreeVariableESDtrackDCAXY;
    Float_t fTreeVariableESDtrackEta;
    Float_t fTreeVariableESDtrackPt;
    Float_t fTreeVariableESDtrackMom;
    Float_t fTreeVariableESDtrackfNclusTPC;
    
    //--Tree Variables for event count--//
    Double_t fTreeVariableEvent;
    Int_t fTreeVariableRunNumber;
    ULong64_t fTreeVariableEventNumber1;
    ULong64_t fTreeVariableEventNumber2;
    ULong64_t fTreeVariableEventNumber3;
//    ULong64_t fTreeVariableEventNumber4;
    Float_t fTreeVariableEventNumberInFile;
    
 /*   //--Tree Variable for MC--//
    Float_t fTreeVariableMCinputTotalXiPt;
    Float_t fTreeVariableMCinputTotalXiY;
    Float_t fTreeVariableMCinputTotalXiCalcMass;
    
    Float_t fTreeVariableMCinputTotalXibarPt;
    Float_t fTreeVariableMCinputTotalXibarY;
    Float_t fTreeVariableMCinputTotalXibarCalcMass;

    Float_t fTreeVariableMCinputTotalXiStarPt;
    Float_t fTreeVariableMCinputTotalXiStarY;
    Float_t fTreeVariableMCinputTotalXiStarCalcMass;

    
    Float_t fTreeVariableMCinputTotalXiStarbarPt;
    Float_t fTreeVariableMCinputTotalXiStarbarY;
    Float_t fTreeVariableMCinputTotalXiStarbarCalcMass;
    
    Int_t fTreeVariableMCinputTotalSongPID;
    Float_t fTreeVariableMCinputTotalCent;
    
 */
   

    
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
        //
       
    };
    struct St_CutType CutVar[kNCutVariations]; //!
    
    
    AliESDtrack* fESDTrack4; //! esdtrack for XiStar's daughter pion
    AliESDtrack* fXiTrack; //! esdtrack for XiStar's daughter Xi
    
    Int_t fCutList;// Cut List option (mean values or systematic variations)
    
    Float_t fDecayParameters[kNCuts];// array of reconstruction kinematics
    Float_t fCutValues[kNCutVariations][kNCuts];// array of reconstruction kinematics
    
    ClassDef(AliXiStar, 1); 
};

#endif
