
/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

////////////////////////////////////////////////////////////////////////////////
//
//  This class is used to reconstruct the neutral Xi(1530) resonance.
//  This class essentially combines charged Xi candidates from the Xi Vertexer
//  with primary charged pions.
//
//  authors: Dhevan Gangadharan (dhevan.raja.gangadharan@cern.ch)
//
////////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <math.h>
#include "TChain.h"
#include "TFile.h"
#include "TKey.h"
#include "TObject.h"
#include "TObjString.h"
#include "TRandom.h"
#include "TList.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TProfile.h"
#include "TCanvas.h"

#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"


#include "AliESDEvent.h"
#include "AliESDInputHandler.h"
#include "AliESDtrackCuts.h"
#include "AliMCEventHandler.h"
#include "AliMCEvent.h"
#include "AliStack.h"
#include "AliCentrality.h"


#include "AliAODEvent.h"
#include "AliAODInputHandler.h"
#include "AliAODMCParticle.h"
#include "AliAODcascade.h"
#include "AliESDcascade.h"
#include "AliV0vertexer.h"
#include "AliCascadeVertexer.h"

#include "AliXiStarPbPb.h"

#define PI 3.1415927


// Author: Dhevan Gangadharan

ClassImp(AliXiStarPbPb)

//________________________________________________________________________
AliXiStarPbPb::AliXiStarPbPb():
AliAnalysisTaskSE(),
fname(0),
fESD(0x0),
fOutputList(0x0),
fTrackCut(0x0),
fPIDResponse(0x0),

fCentrality(0),
//fTreeXi(0),
//fTreePi(0),
//fTreeEvent(0),
//fTreeMC(0),
fEC(0x0),
fEvt(0x0),

fTempStruct(0x0),
fZvertexBins(0),
fEventsToMix(0),
fMultBins(0),
fMCcase(0),
fEventCounter(0),
fEventNumber(0),
fCentralityEstimator("V0M"),

fMaxDecayLength(0),
fMassWindow(0),
fTrueMassPr(0),
fTrueMassPi(0),
fTrueMassK(0),
fTrueMassLam(0),
fTrueMassXi(0),

fESDTrack4(0x0),
fXiTrack(0x0),
fCutList(0)




{
    // Default Constructor
    for (Int_t i=0; i<21; i++){
        fCovMatrix[i]=-99999.;
        if (i<12) fMultLimits[i] = 0;
    }
    for (Int_t i=0; i<kNCuts; i++){
        fDecayParameters[i]=0;
        for (Int_t j=0; j<kNCutVariations; j++){
            fCutValues[j][i]=0;
        }
    }
    //
    for (Int_t cv=0; cv<kNCutVariations; cv++){
        CutVar[cv].fXi=0x0;
        CutVar[cv].fXibar=0x0;
        CutVar[cv].fXiMinusPiPlus=0x0;
        CutVar[cv].fXiMinusPiMinus=0x0;
        CutVar[cv].fXiPlusPiPlus=0x0;
        CutVar[cv].fXiPlusPiMinus=0x0;
        //
        CutVar[cv].fXiMinusPiPlusbkg=0x0;
        CutVar[cv].fXiMinusPiMinusbkg=0x0;
        CutVar[cv].fXiPlusPiPlusbkg=0x0;
        CutVar[cv].fXiPlusPiMinusbkg=0x0;
        //
        CutVar[cv].fMCrecXi=0x0;
        CutVar[cv].fMCrecXibar=0x0;
        CutVar[cv].fMCrecXiMinusPiPlus=0x0;
        CutVar[cv].fMCrecXiPlusPiMinus=0x0;
    }
    
}
//________________________________________________________________________
AliXiStarPbPb::AliXiStarPbPb(const char *name, Bool_t AODdecision,  Bool_t MCdecision, Int_t CutListOption)
: AliAnalysisTaskSE(name),
fname(name),
fESD(0x0),
fOutputList(0x0),
fTrackCut(0x0),
fPIDResponse(0x0),
fCentrality(0),
//fTreeXi(0),
//fTreePi(0),
//fTreeEvent(0),
//fTreeMC(0),
fEC(0x0),
fEvt(0x0),
fTempStruct(0x0),
fZvertexBins(0),
fEventsToMix(0),
fMultBins(0),
fMCcase(MCdecision),
fAODcase(AODdecision),
fEventCounter(0),
fEventNumber(0),
fCentralityEstimator("V0M"),
fMaxDecayLength(0),
fMassWindow(0),
fTrueMassPr(0),
fTrueMassPi(0),
fTrueMassK(0),
fTrueMassLam(0),
fTrueMassXi(0),

fESDTrack4(0x0),
fXiTrack(0x0),
fCutList(CutListOption)



{
    // Main Constructor
    for (Int_t i=0; i<21; i++){
        fCovMatrix[i]=-99999.;
        if (i<12) fMultLimits[i] = 0;
    }
    for (Int_t i=0; i<kNCuts; i++){
        fDecayParameters[i]=0;
        for (Int_t j=0; j<kNCutVariations; j++){
            fCutValues[j][i]=0;
        }
    }
    //
    for (Int_t cv=0; cv<kNCutVariations; cv++){
        CutVar[cv].fXi=0x0;
        CutVar[cv].fXibar=0x0;
        CutVar[cv].fXiMinusPiPlus=0x0;
        CutVar[cv].fXiMinusPiMinus=0x0;
        CutVar[cv].fXiPlusPiPlus=0x0;
        CutVar[cv].fXiPlusPiMinus=0x0;
        //
        CutVar[cv].fXiMinusPiPlusbkg=0x0;
        CutVar[cv].fXiMinusPiMinusbkg=0x0;
        CutVar[cv].fXiPlusPiPlusbkg=0x0;
        CutVar[cv].fXiPlusPiMinusbkg=0x0;
        //
        CutVar[cv].fMCrecXi=0x0;
        CutVar[cv].fMCrecXibar=0x0;
        CutVar[cv].fMCrecXiMinusPiPlus=0x0;
        CutVar[cv].fMCrecXiPlusPiMinus=0x0;
    }
    
    // Define output for Tree
    DefineInput (0, TChain::Class()); //jisong added
    
    // Define output slots here
    // Output slot #1
 //   DefineOutput(0, TTree::Class());
    DefineOutput(1, TList::Class());
    
}
//________________________________________________________________________
AliXiStarPbPb::AliXiStarPbPb(const AliXiStarPbPb &obj)
: AliAnalysisTaskSE(obj.fname),
fname(obj.fname),
fESD(obj.fESD),
fOutputList(obj.fOutputList),
fTrackCut(obj.fTrackCut),
fPIDResponse(obj.fPIDResponse),
fCentrality(obj.fCentrality),
//fTreeXi(obj.fTreeXi),
//fTreePi(obj.fTreePi),
//fTreeEvent(obj.fTreeEvent),
//fTreeMC(obj.fTreeMC),
fEC(obj.fEC),
fEvt(obj.fEvt),
fTempStruct(obj.fTempStruct),
fZvertexBins(obj.fZvertexBins),
fEventsToMix(obj.fEventsToMix),
fMultBins(obj.fMultBins),
fMultLimits(),
fMCcase(obj.fMCcase),
fEventCounter(obj.fEventCounter),
fEventNumber(obj.fEventNumber),
fCentralityEstimator(obj.fCentralityEstimator),
fMaxDecayLength(obj.fMaxDecayLength),
fMassWindow(obj.fMassWindow),
fTrueMassPr(obj.fTrueMassPr),
fTrueMassPi(obj.fTrueMassPi),
fTrueMassK(obj.fTrueMassK),
fTrueMassLam(obj.fTrueMassLam),
fTrueMassXi(obj.fTrueMassXi),

fESDTrack4(obj.fESDTrack4),
fXiTrack(obj.fXiTrack),
fCutList(obj.fCutList)

{
    // Copy constructor
    for (Int_t i=0; i<21; i++){
        fCovMatrix[i]=obj.fCovMatrix[i];
        if (i<12) fMultLimits[i]=obj.fMultLimits[i];
    }
    for (Int_t i=0; i<kNCuts; i++){
        fDecayParameters[i]=obj.fDecayParameters[i];
        for (Int_t j=0; j<kNCutVariations; j++){
            fCutValues[j][i]=obj.fCutValues[j][i];
        }
    }
    
}
//________________________________________________________________________
AliXiStarPbPb &AliXiStarPbPb::operator=(const AliXiStarPbPb &obj)
{
    // Assignment operator
    if (this == &obj)
        return *this;
    
    fname = obj.fname;
    fESD = obj.fESD;
    fOutputList = obj.fOutputList;
    fTrackCut = obj.fTrackCut;
    fPIDResponse = obj.fPIDResponse;
    fCentrality = obj.fCentrality;
 //   fTreeXi = obj.fTreeXi;
 //   fTreePi = obj.fTreePi;
 //   fTreeEvent =  obj.fTreeEvent;
 //   fTreeMC = obj.fTreeMC;
    fEC = obj.fEC;
    fEvt = obj.fEvt;
    fTempStruct = obj.fTempStruct;
    fZvertexBins = obj.fZvertexBins;
    fEventsToMix = obj.fEventsToMix;
    fMultBins = obj.fMultBins;
    for (Int_t i=0; i<12; i++){
        fMultLimits[i]=obj.fMultLimits[i];
    }
    fMCcase = obj.fMCcase;
    fEventCounter = obj.fEventCounter;
    fEventNumber = obj.fEventNumber;
    fMaxDecayLength = obj.fMaxDecayLength;
    fMassWindow = obj.fMassWindow;
    for (Int_t i=0; i<21; i++){
        fCovMatrix[i]=obj.fCovMatrix[i];
    }
    
    fTrueMassPr = obj.fTrueMassPr;
    fTrueMassPi = obj.fTrueMassPi;
    fTrueMassK = obj.fTrueMassK;
    fTrueMassLam = obj.fTrueMassLam;
    fTrueMassXi = obj.fTrueMassXi;
    fESDTrack4 = obj.fESDTrack4;
    fXiTrack = obj.fXiTrack;
    fCutList = obj.fCutList;
    fCentralityEstimator = obj.fCentralityEstimator;
    
    
    for (Int_t i=0; i<kNCuts; i++){
        fDecayParameters[i]=obj.fDecayParameters[i];
        for (Int_t j=0; j<kNCutVariations; j++){
            fCutValues[j][i]=obj.fCutValues[j][i];
        }
    }
    
    
    return (*this);
}
//________________________________________________________________________
AliXiStarPbPb::~AliXiStarPbPb()
{
    // Destructor

    if(fESD) delete fESD;
    if(fOutputList) delete fOutputList;
    if(fTrackCut) delete fTrackCut;
    if(fPIDResponse) delete fPIDResponse;
    if(fCentrality) delete fCentrality;
    if(fEC) delete fEC;
    if(fEvt) delete fEvt;
    if(fTempStruct) delete fTempStruct;
    if(fESDTrack4) delete fESDTrack4;
    if(fXiTrack) delete fXiTrack;
    
    for (Int_t cv=0; cv<kNCutVariations; cv++){
        if(CutVar[cv].fXi) delete CutVar[cv].fXi;
        if(CutVar[cv].fXibar) delete CutVar[cv].fXibar;
        if(CutVar[cv].fXiMinusPiPlus) delete CutVar[cv].fXiMinusPiPlus;
        if(CutVar[cv].fXiMinusPiMinus) delete CutVar[cv].fXiMinusPiMinus;
        if(CutVar[cv].fXiPlusPiPlus) delete CutVar[cv].fXiPlusPiPlus;
        if(CutVar[cv].fXiPlusPiMinus) delete CutVar[cv].fXiPlusPiMinus;
        //
        if(CutVar[cv].fXiMinusPiPlusbkg) delete CutVar[cv].fXiMinusPiPlusbkg;
        if(CutVar[cv].fXiMinusPiMinusbkg) delete CutVar[cv].fXiMinusPiMinusbkg;
        if(CutVar[cv].fXiPlusPiPlusbkg) delete CutVar[cv].fXiPlusPiPlusbkg;
        if(CutVar[cv].fXiPlusPiMinusbkg) delete CutVar[cv].fXiPlusPiMinusbkg;
        //
        if(CutVar[cv].fMCrecXi) delete CutVar[cv].fMCrecXi;
        if(CutVar[cv].fMCrecXibar) delete CutVar[cv].fMCrecXibar;
        if(CutVar[cv].fMCrecXiMinusPiPlus) delete CutVar[cv].fMCrecXiMinusPiPlus;
        if(CutVar[cv].fMCrecXiPlusPiMinus) delete CutVar[cv].fMCrecXiPlusPiMinus;
    }
    
}
//________________________________________________________________________
void AliXiStarPbPb::XiStarInit()
{
    //
    //Inits cuts and analysis settings
    //
    
    fEventCounter=0;// event counter initialization
    cout<<"AliXiStarPbPb XiStarInit() call"<<endl;
    
    
    ///////////////////////////////////////////////
    // Track Cuts for ESD analysis
    fTrackCut = new AliESDtrackCuts();
    fTrackCut->SetPtRange(.15,1000);
    fTrackCut->SetAcceptKinkDaughters(kFALSE);
    //fTrackCut->SetMinNClustersTPC(70);
    fTrackCut->SetRequireTPCRefit(kTRUE);
    fTrackCut->SetMaxChi2PerClusterTPC(4); //From Enrico

    ////////////////////////////////////////////////
    
 //inpPb   fZvertexBins = 20;
 //inpPb   fMultBins = 11;// This must also be set in AliXiStarPbPb.h
    
    fZvertexBins = 10;
   // fMultBins = 2;// for central collisions
     fMultBins = 4;// for semi and MB central collisions

    
    if(fMCcase) fEventsToMix = 0;
    else fEventsToMix = 4; // original 40 jisong
    //fMultLimits[0]=0, fMultLimits[1]=4000, fMultLimits[2]=10000; // for central collisions // this infor from myTrack (fMult5)
    //fMultLimits[0]=0, fMultLimits[1]=900, fMultLimits[2]=1500, fMultLimits[3]=2400, fMultLimits[4]=8000; // for semi-central collisions
    fMultLimits[0]=0, fMultLimits[1]=30, fMultLimits[2]=100, fMultLimits[3]=300, fMultLimits[4]=6000; // for MB


    
    
    // multiplicity edges for event mixing bins // multiplicity method
//inpPb    fMultLimits[0]=0, fMultLimits[1]=15, fMultLimits[2]=30, fMultLimits[3]=45, fMultLimits[4]=60, fMultLimits[5]=75;
//inpPb    fMultLimits[6]=90, fMultLimits[7]=105, fMultLimits[8]=130, fMultLimits[9]=170, fMultLimits[10]=200, fMultLimits[11]=900;
    
    // multiplicity edges for event mixing bins // multiplicity method // old : before 2014 May 25
    //fMultLimits[0]=0, fMultLimits[1]=150, fMultLimits[2]=300, fMultLimits[3]=450, fMultLimits[4]=600, fMultLimits[5]=750;
    //fMultLimits[6]=900, fMultLimits[7]=1050, fMultLimits[8]=1200, fMultLimits[9]=1300, fMultLimits[10]=1400, fMultLimits[11]=1500;
    
    
    /*    fMultLimits[0]=0, fMultLimits[1]=5, fMultLimits[2]=10, fMultLimits[3]=15, fMultLimits[4]=20, fMultLimits[5]=25;
     fMultLimits[6]=30, fMultLimits[7]=35, fMultLimits[8]=40, fMultLimits[9]=45, fMultLimits[10]=50, fMultLimits[11]=150;
     */
    
    fEC = new AliXiStarPbPbEventCollection **[fZvertexBins];
    for(unsigned short i=0; i<fZvertexBins; i++){
        
        fEC[i] = new AliXiStarPbPbEventCollection *[fMultBins];
        
        for(unsigned short j=0; j<fMultBins; j++){
            
            fEC[i][j] = new AliXiStarPbPbEventCollection(fEventsToMix+1);
        }
    }
    
    fTempStruct = new AliXiStarPbPbTrackStruct[20000];
    
    
    
    fESDTrack4 = new AliESDtrack();
    fXiTrack = new AliESDtrack();
    
    
    fMaxDecayLength = 100.;
    fMassWindow = 0.007;
    
    /////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////
    // DecayParameters Key (number represents array index)
    // NclustersTPC: 0=proton, 1=pion first, 2=pion second, 3=pion third
    // DCAVtx: 4=proton, 5=pion first, 6=pion second, 7=lambda, 8=pion third
    // 9 = DCA proton-pion
    // 10 = DCA Lambda-pion
    // 11 = Cos PA Lambda
    // 12 = Cos PA Xi
    
    // Set Standard Reconstruction cut values
    fCutValues[0][0] = 70; fCutValues[0][1] = 70; fCutValues[0][2] = 70; fCutValues[0][3] = 70;
    fCutValues[0][4] = 0.11;
    fCutValues[0][5] = 0.11;
    fCutValues[0][6] = 0.035;
    fCutValues[0][7] = 0.06;
    fCutValues[0][8] = 2.5;
    fCutValues[0][9] = 0.95;
    fCutValues[0][10] = 0.275;
    fCutValues[0][11] = 0.95;
    fCutValues[0][12] = 0.9992;
    
    
    for(int cv=1; cv<kNCutVariations; cv++){
        for(int ct=0; ct<kNCuts; ct++){
            fCutValues[cv][ct] = fCutValues[0][ct];
        }
    }
    
    //systematic variation// Loose
    fCutValues[1][0] = 63; fCutValues[1][1] = 63; fCutValues[1][2] = 63; fCutValues[1][3] = 63;// 80
    fCutValues[2][4] = 0.1;
    fCutValues[3][5] = 0.1;
    fCutValues[4][6] = 0.03;
    fCutValues[5][7] = 0.05;
    fCutValues[6][8] = 3.0;
    fCutValues[7][9] = 1.0;
    fCutValues[8][10] = 0.3;
    fCutValues[9][11] = 0.94;
    fCutValues[10][12] = 0.999;

    //systematic variation// tight
    fCutValues[11][0] = 77; fCutValues[11][1] = 77; fCutValues[11][2] = 77; fCutValues[11][3] = 77;// 80
    fCutValues[12][4] = 0.12;
    fCutValues[13][5] = 0.12;
    fCutValues[14][6] = 0.04;
    fCutValues[15][7] = 0.07;
    fCutValues[16][8] = 2.0;
    fCutValues[17][9] = 0.9;
    fCutValues[18][10] = 0.25;
    fCutValues[19][11] = 0.96;
    fCutValues[20][12] = 0.9994;

    
    
    
    // PDG mass values
    fTrueMassPr=.93827, fTrueMassPi=.13957, fTrueMassK=.493677, fTrueMassLam=1.11568, fTrueMassXi=1.32171;
    
    // The following CovMatrix is set so that PropogateToDCA() ignores track errors. Only used to propagate Xi to third pion for XiStar reconstruction
    for(Int_t i=0; i<21; i++) fCovMatrix[i]=0;
    fCovMatrix[0]=1, fCovMatrix[2]=1, fCovMatrix[5]=1, fCovMatrix[9]=1, fCovMatrix[14]=1, fCovMatrix[20]=1;
    
    
}
//________________________________________________________________________
void AliXiStarPbPb::UserCreateOutputObjects()
{
    XiStarInit();
    // XiStarInit();// Initialize settings original
    
    // Create histograms
    fOutputList = new TList();
    fOutputList->SetOwner();
    
    TH3F *fVertexDist1 = new TH3F("fVertexDist1","Vertex Distribution",20,-1,1, 20,-1,1, 600,-30,30);
    fVertexDist1->GetXaxis()->SetTitle("X Vertex (cm)");
    fVertexDist1->GetYaxis()->SetTitle("Y Vertex (cm)");
    fVertexDist1->GetZaxis()->SetTitle("Z Vertex (cm)");
    fOutputList->Add(fVertexDist1);
    
    TH3F *fVertexDist3 = new TH3F("fVertexDist3","Vertex Distribution",20,-1,1, 20,-1,1, 600,-30,30);
    fVertexDist3->GetXaxis()->SetTitle("X Vertex (cm)");
    fVertexDist3->GetYaxis()->SetTitle("Y Vertex (cm)");
    fVertexDist3->GetZaxis()->SetTitle("Z Vertex (cm)");
    fOutputList->Add(fVertexDist3);
    
    TH2F *fDCADist = new TH2F("fDCADist","DCA distribution",kNbinsM,-.5,kNbinsM*8-.5, 100,0,10);
    fOutputList->Add(fDCADist);
    
    
  /*  TH3F *fMultDist3d = new TH3F("fMultDist3d","Multiplicity Distribution",kNbinsM,-.5,kNbinsM-.5, kNbinsM,-.5,kNbinsM-.5, kNbinsM,-.5,kNbinsM-.5);
    fMultDist3d->GetXaxis()->SetTitle("Multiplicity");
    fMultDist3d->GetYaxis()->SetTitle("Positive Multiplicity");
    fMultDist3d->GetZaxis()->SetTitle("Negative Multiplicity");
    fMultDist3d->SetMarkerStyle(kFullCircle);
    fOutputList->Add(fMultDist3d);
 */
    
    TH1F *fMultDist1 = new TH1F("fMultDist1","Multiplicity Distribution",kNbinsM,0,kNbinsM*100);
    fMultDist1->GetXaxis()->SetTitle("Multiplicity");
    fOutputList->Add(fMultDist1);
    
    TH1F *fMultDist2 = new TH1F("fMultDist2","Multiplicity Distribution After Vz selection",kNbinsM,-.5,kNbinsM*100-.5);
    fMultDist2->GetXaxis()->SetTitle("Multiplicity");
    fOutputList->Add(fMultDist2);
    
    TH1F *fMultDist3 = new TH1F("fMultDist3","Multiplicity Distribution After reject pile up from SPD",kNbinsM,-.5,kNbinsM*100-.5);
    fMultDist3->GetXaxis()->SetTitle("Multiplicity");
    fOutputList->Add(fMultDist3);
    
    TH1F *fMultDist4 = new TH1F("fMultDist4","Multiplicity Distribution : Primary NContributor > 1 ",kNbinsM,-.5,kNbinsM*50-.5);
    fMultDist4->GetXaxis()->SetTitle("Multiplicity");
    fOutputList->Add(fMultDist4);
    
    TH1F *fMultDist5 = new TH1F("fMultDist5","Multiplicity Distribution of mytrack",kNbinsM*10,-.5,kNbinsM*50-.5);
    fMultDist5->GetXaxis()->SetTitle("Multiplicity");
    fOutputList->Add(fMultDist5);
    
    TH1F *fMultDist6 = new TH1F("fMultDist6","Multiplicity Distribution",kNbinsM,-.5,kNbinsM*100-.5);
    fMultDist6->GetXaxis()->SetTitle("Multiplicity");
    fOutputList->Add(fMultDist6);
    
    TH1F *hCentrality = new TH1F("hCentrality","Centrality",100,0,100);
    hCentrality->GetXaxis()->SetTitle("Centrality");
    fOutputList->Add(hCentrality);
    
    TH1F *hCentralityNoFlat = new TH1F("hCentralityNoFlat","hCentralityNoFlat",100,0,100);
    hCentralityNoFlat->GetXaxis()->SetTitle("Centrality");
    fOutputList->Add(hCentralityNoFlat);
    
    
    TH1F *hCentralitySelected = new TH1F("hCentralitySelected","Centrality : after MB event selection",100,0,100);
    hCentralitySelected->GetXaxis()->SetTitle("Centrality");
    fOutputList->Add(hCentralitySelected);
    
    
    TH1F *hCentralitySelected5090 = new TH1F("hCentralitySelected5090","Centrality : after MB event selection",100,0,100);
    hCentralitySelected5090->GetXaxis()->SetTitle("Centrality");
    fOutputList->Add(hCentralitySelected5090);
    
    TH1F *hCentralityNoFlatSelected = new TH1F("hCentralityNoFlatSelected","hCentralityNoFlatSelected",100,0,100);
    hCentralityNoFlatSelected->GetXaxis()->SetTitle("Centrality");
    fOutputList->Add(hCentralityNoFlatSelected);
    


    TH1F *hEventSelecInfo = new TH1F("hEventSelecInfo","hEventSelecInfo",10,0,10);
    fOutputList->Add(hEventSelecInfo);
    hEventSelecInfo->GetXaxis()->SetBinLabel(2,"kMB");
    hEventSelecInfo->GetXaxis()->SetBinLabel(3,"kINT7");
    hEventSelecInfo->GetXaxis()->SetBinLabel(8,"kAny");
    hEventSelecInfo->GetXaxis()->SetBinLabel(9,"kAndMB");

    
   // TH3F *fPtEtaDist = new TH3F("fPtEtaDist","PtEtaDist",2,-1.1,1.1, 300,0,3., 28,-1.4,1.4);
   // fOutputList->Add(fPtEtaDist);
    
   // TH3F *fPhiPtDist = new TH3F("fPhiPtDist","PhiPtDist",2,-1.1,1.1, 120,0,2*PI, 300,0,3.);
   // fOutputList->Add(fPhiPtDist);
    
    
    TH1F *fPtDist = new TH1F("fPtDist","fPtDist",90,0,9);
    fPtDist->GetXaxis()->SetTitle("fPtDist");
    fOutputList->Add(fPtDist);
    
    
    TH1F *fPhiDist = new TH1F("fPhiDist","fPhiDist",70,0,7);
    fPhiDist->GetXaxis()->SetTitle("fPhiDist");
    fOutputList->Add(fPhiDist);
    
    
    TH1F *fEtaDist = new TH1F("fEtaDist","fEtaDist",60,-3,3);
    fEtaDist->GetXaxis()->SetTitle("fEtaDist");
    fOutputList->Add(fEtaDist);
    
    TH1F *fXiStarYDist = new TH1F("fXiStarYDist","fXiStarYDist",400,-2,2);
    fXiStarYDist->GetXaxis()->SetTitle("fXiStarYDist");
    fOutputList->Add(fXiStarYDist);
    
    TH1F *fQAXiStarYDist = new TH1F("fQAXiStarYDist","fQAXiStarYDist",400,-2,2);
    fQAXiStarYDist->GetXaxis()->SetTitle("fQAXiStarYDist");
    fOutputList->Add(fQAXiStarYDist);
    
    
    TH1F *fXiStarYDistMC = new TH1F("fXiStarYDistMC","fXiStarYDist in MC",400,-2,2);
    fXiStarYDistMC->GetXaxis()->SetTitle("fXiStarYDistMC");
    fOutputList->Add(fXiStarYDistMC);
    
    TH1F *fQAXiStarYDistMC = new TH1F("fQAXiStarYDistMC","fQAXiStarYDist in MC",400,-2,2);
    fQAXiStarYDistMC->GetXaxis()->SetTitle("fQAXiStarYDistMC");
    fOutputList->Add(fQAXiStarYDistMC);

    TH1F *fXiStarYDistMCout = new TH1F("fXiStarYDistMCout","fXiStarYDist output in MC",400,-2,2);
    fXiStarYDistMCout->GetXaxis()->SetTitle("fXiStarYDistMCout");
    fOutputList->Add(fXiStarYDistMCout);
    
    TH1F *fCutEvents = new TH1F("fCutEvents","fCutEvents",16,0,16);
    fOutputList->Add(fCutEvents);
    
    TH1F *fTPCNcls_p = new TH1F("fTPCNcls_p","TPC Number of cluster proton",200,0,200); //par 0
    fOutputList->Add(fTPCNcls_p);
    TH1F *fTPCNcls_pi1 = new TH1F("fTPCNcls_pi1","TPC Number of cluster 1st pion",200,0,200); //par 1
    fOutputList->Add(fTPCNcls_pi1);
    TH1F *fTPCNcls_pi2 = new TH1F("fTPCNcls_pi2","TPC Number of cluster 2nd pion",200,0,200); //par 2
    fOutputList->Add(fTPCNcls_pi2);
    TH1F *fTPCNcls_pi3 = new TH1F("fTPCNcls_pi3","TPC Number of cluster 3rd pion",200,0,200); //par 3
    fOutputList->Add(fTPCNcls_pi3);
    
    
    TH1F *fQATPCNcls_p = new TH1F("fQATPCNcls_p"," After cut : TPC Number of cluster proton",200,0,200);
    fOutputList->Add(fQATPCNcls_p);
    TH1F *fQATPCNcls_pi1 = new TH1F("fQATPCNcls_pi1","  After cut : TPC Number of cluster 1st pion",200,0,200);
    fOutputList->Add(fQATPCNcls_pi1);
    TH1F *fQATPCNcls_pi2 = new TH1F("fQATPCNcls_pi2","  After cut : TPC Number of cluster 2nd pion",200,0,200);
    fOutputList->Add(fQATPCNcls_pi2);
    TH1F *fQATPCNcls_pi3 = new TH1F("fQATPCNcls_pi3","  After cut : TPC Number of cluster 3rd pion",200,0,200);
    fOutputList->Add(fQATPCNcls_pi3);
    
    
    TH1F *fQATPCNcls_p_L = new TH1F("fQATPCNcls_p_L"," After loose cut : TPC Number of cluster proton",200,0,200);
    fOutputList->Add(fQATPCNcls_p_L);
    TH1F *fQATPCNcls_pi1_L = new TH1F("fQATPCNcls_pi1_L","  After loose cut : TPC Number of cluster 1st pion",200,0,200);
    fOutputList->Add(fQATPCNcls_pi1_L);
    TH1F *fQATPCNcls_pi2_L = new TH1F("fQATPCNcls_pi2_L","  After loose cut : TPC Number of cluster 2nd pion",200,0,200);
    fOutputList->Add(fQATPCNcls_pi2_L);
    TH1F *fQATPCNcls_pi3_L = new TH1F("fQATPCNcls_pi3_L","  After loose cut : TPC Number of cluster 3rd pion",200,0,200);
    fOutputList->Add(fQATPCNcls_pi3_L);
    
    
    TH1F *fQATPCNcls_p_T = new TH1F("fQATPCNcls_p_T"," After tight cut : TPC Number of cluster proton",200,0,200);
    fOutputList->Add(fQATPCNcls_p_T);
    TH1F *fQATPCNcls_pi1_T = new TH1F("fQATPCNcls_pi1_T","  After tight cut : TPC Number of cluster 1st pion",200,0,200);
    fOutputList->Add(fQATPCNcls_pi1_T);
    TH1F *fQATPCNcls_pi2_T = new TH1F("fQATPCNcls_pi2_T","  After tight cut : TPC Number of cluster 2nd pion",200,0,200);
    fOutputList->Add(fQATPCNcls_pi2_T);
    TH1F *fQATPCNcls_pi3_T = new TH1F("fQATPCNcls_pi3_T","  After tight cut : TPC Number of cluster 3rd pion",200,0,200);
    fOutputList->Add(fQATPCNcls_pi3_T);
    
    
    
    
    
    TH1F *fDCADist_p = new TH1F("fDCADist_p","DCA distribution proton",200,0, 0.5); //par 4
    fOutputList->Add(fDCADist_p);
    TH1F *fDCADist_pi1 = new TH1F("fDCADist_pi1","DCA distribution 1st pion",200,0, 0.5); //par 5
    fOutputList->Add(fDCADist_pi1);
    TH1F *fDCADist_pi2 = new TH1F("fDCADist_pi2","DCA distribution 2nd pion",200,0, 0.5); //par 6
    fOutputList->Add(fDCADist_pi2);
    
    TH1F *fQADCADist_p = new TH1F("fQADCADist_p","After cut :DCA distribution proton",200,0, 0.5); //par 4
    fOutputList->Add(fQADCADist_p);
    TH1F *fQADCADist_pi1 = new TH1F("fQADCADist_pi1","After cut :DCA distribution 1st pion",200,0, 0.5); //par 5
    fOutputList->Add(fQADCADist_pi1);
    TH1F *fQADCADist_pi2 = new TH1F("fQADCADist_pi2","After cut :DCA distribution 2nd pion",200,0, 0.5); //par 6
    fOutputList->Add(fQADCADist_pi2);
    
    TH1F *fQADCADist_p_L = new TH1F("fQADCADist_p_L","After loose cut :DCA distribution proton",200,0, 0.5); //par 4
    fOutputList->Add(fQADCADist_p_L);
    TH1F *fQADCADist_pi1_L = new TH1F("fQADCADist_pi1_L","After loose cut :DCA distribution 1st pion",200,0, 0.5); //par 5
    fOutputList->Add(fQADCADist_pi1_L);
    TH1F *fQADCADist_pi2_L = new TH1F("fQADCADist_pi2_L","After loose cut :DCA distribution 2nd pion",200,0, 0.5); //par 6
    fOutputList->Add(fQADCADist_pi2_L);
    
    TH1F *fQADCADist_p_T = new TH1F("fQADCADist_p_T","After tight cut :DCA distribution proton",200,0, 0.5); //par 4
    fOutputList->Add(fQADCADist_p_T);
    TH1F *fQADCADist_pi1_T = new TH1F("fQADCADist_pi1_T","After tight cut :DCA distribution 1st pion",200,0, 0.5); //par 5
    fOutputList->Add(fQADCADist_pi1_T);
    TH1F *fQADCADist_pi2_T = new TH1F("fQADCADist_pi2_T","After tight cut :DCA distribution 2nd pion",200,0, 0.5); //par 6
    fOutputList->Add(fQADCADist_pi2_T);

    
    
    TH1F *fDCADist_lambda = new TH1F("fDCADist_lambda","DCA distribution Lambda",200,0,0.5);
    fOutputList->Add(fDCADist_lambda);
    TH1F *fDCADist_3rd_pi = new TH1F("fDCADist_3rd_pi","DCA distribution 3rd pion",300,0,3);
    fOutputList->Add(fDCADist_3rd_pi);
    TH1F *fDCADist_pi_p = new TH1F("fDCADist_pi_p","DCA distribution Pion-Proton",300,0,3);
    fOutputList->Add(fDCADist_pi_p);
    TH1F *fDCADist_pi_lambda = new TH1F("fDCADist_pi_lambda","DCA distribution Pion-Lambda",300,0,3);
    fOutputList->Add(fDCADist_pi_lambda);
    
    
    TH1F *fQADCADist_lambda = new TH1F("fQADCADist_lambda","After cut :DCA distribution Lambda",200,0,0.5);//7
    fOutputList->Add(fQADCADist_lambda);
    TH1F *fQADCADist_3rd_pi = new TH1F("fQADCADist_3rd_pi","After cut :DCA distribution 3rd pion",300,0,3); //8
    fOutputList->Add(fQADCADist_3rd_pi);
    TH1F *fQADCADist_pi_p = new TH1F("fQADCADist_pi_p","After cut :DCA distribution Pion-Proton",300,0,3); //9
    fOutputList->Add(fQADCADist_pi_p);
    TH1F *fQADCADist_pi_lambda = new TH1F("fQADCADist_pi_lambda","After cut :DCA distribution Pion-Lambda",300,0,3); //10
    fOutputList->Add(fQADCADist_pi_lambda);
    
    
    TH1F *fQADCADist_lambda_L = new TH1F("fQADCADist_lambda_L","After loose cut :DCA distribution Lambda",200,0,0.5);//7
    fOutputList->Add(fQADCADist_lambda_L);
    TH1F *fQADCADist_3rd_pi_L = new TH1F("fQADCADist_3rd_pi_L","After loose cut :DCA distribution 3rd pion",300,0,3); //8
    fOutputList->Add(fQADCADist_3rd_pi_L);
    TH1F *fQADCADist_pi_p_L = new TH1F("fQADCADist_pi_p_L","After loose cut :DCA distribution Pion-Proton",300,0,3); //9
    fOutputList->Add(fQADCADist_pi_p_L);
    TH1F *fQADCADist_pi_lambda_L = new TH1F("fQADCADist_pi_lambda_L","After loose cut :DCA distribution Pion-Lambda",300,0,3); //10
    fOutputList->Add(fQADCADist_pi_lambda_L);
    
    TH1F *fQADCADist_lambda_T = new TH1F("fQADCADist_lambda_T","After tight cut :DCA distribution Lambda",200,0,0.5);//7
    fOutputList->Add(fQADCADist_lambda_T);
    TH1F *fQADCADist_3rd_pi_T = new TH1F("fQADCADist_3rd_pi_T","After tight cut :DCA distribution 3rd pion",300,0,3); //8
    fOutputList->Add(fQADCADist_3rd_pi_T);
    TH1F *fQADCADist_pi_p_T = new TH1F("fQADCADist_pi_p_T","After tight cut :DCA distribution Pion-Proton",300,0,3); //9
    fOutputList->Add(fQADCADist_pi_p_T);
    TH1F *fQADCADist_pi_lambda_T = new TH1F("fQADCADist_pi_lambda_T","After tight cut :DCA distribution Pion-Lambda",300,0,3); //10
    fOutputList->Add(fQADCADist_pi_lambda_T);
    
     TH1F *fCosPA_lambda = new TH1F("fCosPA_lambda","Cosine pointing angle Lambda",150,0.85,1.0);
    fOutputList->Add(fCosPA_lambda);
    TH1F *fCosPA_Xi = new TH1F("fCosPA_Xi","Cosine pointing angle Xi",125,0.975,1.0);
    fOutputList->Add(fCosPA_Xi);
    
 
    TH1F *fQACosPA_lambda = new TH1F("fQACosPA_lambda","After cut :Cosine pointing angle Lambda",150,0.85,1.0);
    fOutputList->Add(fQACosPA_lambda);
    TH1F *fQACosPA_Xi = new TH1F("fQACosPA_Xi","After cut :Cosine pointing angle Xi",125,0.975,1.0);
    fOutputList->Add(fQACosPA_Xi);
    
     TH1F *fQACosPA_lambda_L = new TH1F("fQACosPA_lambda_L","After loose cut :Cosine pointing angle Lambda",150,0.85,1.0);
    fOutputList->Add(fQACosPA_lambda_L);
    TH1F *fQACosPA_Xi_L = new TH1F("fQACosPA_Xi_L","After loose cut :Cosine pointing angle Xi",125,0.975,1.0);
    fOutputList->Add(fQACosPA_Xi_L);


    TH1F *fQACosPA_lambda_T = new TH1F("fQACosPA_lambda_T","After tight cut :Cosine pointing angle Lambda",150,0.85,1.0);
    fOutputList->Add(fQACosPA_lambda_T);
    TH1F *fQACosPA_Xi_T = new TH1F("fQACosPA_Xi_T","After tight cut :Cosine pointing angle Xi",125,0.975,1.0);
    fOutputList->Add(fQACosPA_Xi_T);

    

    
    
    
    TH1F *hXiInvMass = new TH1F("hXiInvMass","Xi invariant mass distribution : cent 0 - 10",200,1.2,1.4);
    fOutputList->Add(hXiInvMass);
    
    TH1F *hQAXiInvMass = new TH1F("hQAXiInvMass","Xi invariant mass distribution after mass window selection : cent 0 - 10",200,1.2,1.4);
    fOutputList->Add(hQAXiInvMass);
    
   
    
    
    TH2F *TPCPID = new TH2F("TPCPID","PID via TPC",1000,0,20,1000,0,200);
    fOutputList->Add(TPCPID);
    TH2F *hTPCPIDpi = new TH2F("hTPCPIDpi","PID pion",1000,0,20,1000,0,200);
    fOutputList->Add(hTPCPIDpi);
    TH2F *hTPCPIDk = new TH2F("hTPCPIDk","PID kaon",1000,0,20,1000,0,200);
    fOutputList->Add(hTPCPIDk);
    TH2F *hTPCPIDp = new TH2F("hTPCPIDp","PID proton",1000,0,20,1000,0,200);
    fOutputList->Add(hTPCPIDp);
    
    
    
    
    TH2F *hdEdxProton = new TH2F("hdEdxProton","Xi PID p",1000,0,20,1000,0,200);
    fOutputList->Add(hdEdxProton);
    TH2F *hdEdxPion1 = new TH2F("hdEdxPion1","Xi PID pi",1000,0,20,1000,0,200);
    fOutputList->Add(hdEdxPion1);
    TH2F *hdEdxPion2 = new TH2F("hdEdxPion2","Xi PID pi_b",1000,0,20,1000,0,200);
    fOutputList->Add(hdEdxPion2);
    
    TH2F *hdEdxProtonAfter = new TH2F("hdEdxProtonAfter","Xi PID p",1000,0,20,1000,0,200);
    fOutputList->Add(hdEdxProtonAfter);
    TH2F *hdEdxPion1After = new TH2F("hdEdxPion1After","Xi PID pi",1000,0,20,1000,0,200);
    fOutputList->Add(hdEdxPion1After);
    TH2F *hdEdxPion2After = new TH2F("hdEdxPion2After","Xi PID pi_b",1000,0,20,1000,0,200);
    fOutputList->Add(hdEdxPion2After);
    
    
    
    
    TH2F *hNSig3rdPion = new TH2F("hNSig3rdPion","nSigma 3rd pion",1000,0,20,1000,-5,5);
    fOutputList->Add(hNSig3rdPion);
    TH2F *hQANSig3rdPion = new TH2F("hQANSig3rdPion","nSigma 3rd pion : QA",1000,0,20,1000,-5,5);
    fOutputList->Add(hQANSig3rdPion);
    
    TH1F *fTPCNSigProton = new TH1F("fTPCNSigProton","nSigma proton",100,-5,5);
    fOutputList->Add(fTPCNSigProton);
    TH1F *fTPCNSigPion1 = new TH1F("fTPCNSigPion1","nSigma 1st pion",100,-5,5);
    fOutputList->Add(fTPCNSigPion1);
    TH1F *fTPCNSigPion2 = new TH1F("fTPCNSigPion2","nSigma 2nd pion",100,-5,5);
    fOutputList->Add(fTPCNSigPion2);
    
    TH1F *fQATPCNSigProton = new TH1F("fQATPCNSigProton","nSigma proton : QA",100,-5,5);
    fOutputList->Add(fQATPCNSigProton);
    TH1F *fQATPCNSigPion1 = new TH1F("fQATPCNSigPion1","nSigma 1st pion : QA",100,-5,5);
    fOutputList->Add(fQATPCNSigPion1);
    TH1F *fQATPCNSigPion2 = new TH1F("fQATPCNSigPion2","nSigma 2nd pion : QA",100,-5,5);
    fOutputList->Add(fQATPCNSigPion2);
    
    
    
    for(Int_t cv=0; cv<kNCutVariations; cv++){
        
        if(cv==0){
            TString *nameXi=new TString("fXi_");
            TString *nameXibar=new TString("fXibar_");
            *nameXi += cv;
            *nameXibar += cv;
            CutVar[cv].fXi = new TH3F(nameXi->Data(),"Invariant Mass Distribution", 100,0,10, 10, 50,100, 200,1.2,1.4);
            fOutputList->Add(CutVar[cv].fXi);
            CutVar[cv].fXibar = new TH3F(nameXibar->Data(),"Invariant Mass Distribution", 100,0,10, 10, 50,100, 200,1.2,1.4);
           fOutputList->Add(CutVar[cv].fXibar);
            //
            TString *nameMCrecXi = new TString("fMCrecXi_");
            TString *nameMCrecXibar = new TString("fMCrecXibar_");
            *nameMCrecXi += cv;
            *nameMCrecXibar += cv;
            CutVar[cv].fMCrecXi = new TH3F(nameMCrecXi->Data(),"Invariant Mass Distribution", 100,0,10, 10, 50,100, 200,1.2,1.4);
           CutVar[cv].fMCrecXibar = new TH3F(nameMCrecXibar->Data(),"Invariant Mass Distribution", 100,0,10, 10, 50,100, 200,1.2,1.4);
            fOutputList->Add(CutVar[cv].fMCrecXi);
            fOutputList->Add(CutVar[cv].fMCrecXibar);
        }
        //
        TString *nameXiMinusPiPlus = new TString("fXiMinusPiPlus_");
        TString *nameXiMinusPiMinus = new TString("fXiMinusPiMinus_");
        TString *nameXiPlusPiPlus = new TString("fXiPlusPiPlus_");
        TString *nameXiPlusPiMinus = new TString("fXiPlusPiMinus_");
        TString *nameXiMinusPiPlusbkg = new TString("fXiMinusPiPlusbkg_");
        TString *nameXiMinusPiMinusbkg = new TString("fXiMinusPiMinusbkg_");
        TString *nameXiPlusPiPlusbkg = new TString("fXiPlusPiPlusbkg_");
        TString *nameXiPlusPiMinusbkg = new TString("fXiPlusPiMinusbkg_");
        *nameXiMinusPiPlus += cv;
        *nameXiMinusPiMinus += cv;
        *nameXiPlusPiPlus += cv;
        *nameXiPlusPiMinus += cv;
        *nameXiMinusPiPlusbkg += cv;
        *nameXiMinusPiMinusbkg += cv;
        *nameXiPlusPiPlusbkg += cv;
        *nameXiPlusPiMinusbkg += cv;
        CutVar[cv].fXiMinusPiPlus  = new TH3F(nameXiMinusPiPlus->Data(),"Invariant Mass Distribution", 100,0,10,10, 50, 100, 600, 1.4, 2.0);
        CutVar[cv].fXiMinusPiMinus = new TH3F(nameXiMinusPiMinus->Data(),"Invariant Mass Distribution", 100,0,10,10, 50, 100, 600, 1.4, 2.0);
        CutVar[cv].fXiPlusPiPlus   = new TH3F(nameXiPlusPiPlus->Data(),"Invariant Mass Distribution", 100,0,10,10, 50, 100, 600, 1.4, 2.0);
        CutVar[cv].fXiPlusPiMinus  = new TH3F(nameXiPlusPiMinus->Data(),"Invariant Mass Distribution", 100,0,10,10, 50, 100, 600, 1.4, 2.0);
        CutVar[cv].fXiMinusPiPlusbkg  = new TH3F(nameXiMinusPiPlusbkg->Data(),"Invariant Mass Distribution", 100,0,10,10, 50, 100, 600, 1.4, 2.0);
        CutVar[cv].fXiMinusPiMinusbkg = new TH3F(nameXiMinusPiMinusbkg->Data(),"Invariant Mass Distribution", 100,0,10,10, 50, 100, 600, 1.4, 2.0);
        CutVar[cv].fXiPlusPiPlusbkg   = new TH3F(nameXiPlusPiPlusbkg->Data(),"Invariant Mass Distribution", 100,0,10,10, 50, 100, 600, 1.4, 2.0);
        CutVar[cv].fXiPlusPiMinusbkg  = new TH3F(nameXiPlusPiMinusbkg->Data(),"Invariant Mass Distribution", 100,0,10,10, 50, 100, 600, 1.4, 2.0);
        
        fOutputList->Add(CutVar[cv].fXiMinusPiPlus);
        fOutputList->Add(CutVar[cv].fXiMinusPiMinus);
        fOutputList->Add(CutVar[cv].fXiPlusPiPlus);
        fOutputList->Add(CutVar[cv].fXiPlusPiMinus);
        fOutputList->Add(CutVar[cv].fXiMinusPiPlusbkg);
        fOutputList->Add(CutVar[cv].fXiMinusPiMinusbkg);
        fOutputList->Add(CutVar[cv].fXiPlusPiPlusbkg);
        fOutputList->Add(CutVar[cv].fXiPlusPiMinusbkg);
        //
        
        
        TString *nameMCrecXiMinusPiPlus = new TString("fMCrecXiMinusPiPlus_");
        TString *nameMCrecXiPlusPiMinus = new TString("fMCrecXiPlusPiMinus_");
        *nameMCrecXiMinusPiPlus += cv;
        *nameMCrecXiPlusPiMinus += cv;
        CutVar[cv].fMCrecXiMinusPiPlus  = new TH3F(nameMCrecXiMinusPiPlus->Data(),"Invariant Mass Distribution", 100,0,10, 10,50,100, 600,1.4,2.0);
        CutVar[cv].fMCrecXiPlusPiMinus  = new TH3F(nameMCrecXiPlusPiMinus->Data(),"Invariant Mass Distribution", 100,0,10, 10,50,100, 600,1.4,2.0);
        fOutputList->Add(CutVar[cv].fMCrecXiMinusPiPlus);
        fOutputList->Add(CutVar[cv].fMCrecXiPlusPiMinus);
        //
        
         }
    
    
    
    
    //////////////////////
    // MC input histos
   
    
    //
    
    TH3F *fMCinputTotalXiStar1 = new TH3F("fMCinputTotalXiStar1","Invariant Mass Distribution", 100,0,10, 10, 50,100, 600,1.4,2.0);
    TH3F *fMCinputTotalXiStarbar1 = new TH3F("fMCinputTotalXiStarbar1","Invariant Mass Distribution", 100,0,10, 10, 50,100, 600,1.4,2.0);
    fOutputList->Add(fMCinputTotalXiStar1);
    fOutputList->Add(fMCinputTotalXiStarbar1);
    
    TH3F *fMCinputTotalXi1 = new TH3F("fMCinputTotalXi1","Invariant Mass Distribution", 100,0,10, 10, 50,100, 200,1.2,1.4);
    TH3F *fMCinputTotalXibar1 = new TH3F("fMCinputTotalXibar1","Invariant Mass Distribution", 100,0,10, 10, 50,100, 200,1.2,1.4);
    fOutputList->Add(fMCinputTotalXi1);
    fOutputList->Add(fMCinputTotalXibar1);
    
    //
    
    TH3F *fMCinputTotalXiStar3 = new TH3F("fMCinputTotalXiStar3","Invariant Mass Distribution", 100,0,10, 10, 50,100, 600,1.4,2.0);
    TH3F *fMCinputTotalXiStarbar3 = new TH3F("fMCinputTotalXiStarbar3","Invariant Mass Distribution", 100,0,10, 10, 50,100, 600,1.4,2.0);
    fOutputList->Add(fMCinputTotalXiStar3);
    fOutputList->Add(fMCinputTotalXiStarbar3);
    
    TH3F *fMCinputTotalXi3 = new TH3F("fMCinputTotalXi3","Invariant Mass Distribution", 100,0,10, 10, 50,100, 200,1.2,1.4);
    TH3F *fMCinputTotalXibar3 = new TH3F("fMCinputTotalXibar3","Invariant Mass Distribution", 100,0,10, 10, 50,100, 200,1.2,1.4);
    fOutputList->Add(fMCinputTotalXi3);
    fOutputList->Add(fMCinputTotalXibar3);
    
    //
  
   
    
    ///////////////////////////////////
    PostData(1, fOutputList);
    
    
}

//________________________________________________________________________
void AliXiStarPbPb::Exec(Option_t *)
{
    
  //  fMCcase = kTRUE;
    
    // Main loop
    // Called for each event
    
    cout<<"===========  Event # "<<fEventCounter+1<<"  ==========="<<endl;
    fEventCounter++;
    
    
         fESD = dynamic_cast<AliESDEvent*> (InputEvent());
    

        
        if (!fESD) {Printf("ERROR: fESD not available"); return;}
    
    
    // V0A and V0C time information //
    
  
    
    // centrality selection
    Float_t  centralityV0MnoFlat = -100;
    
    Double_t centralityV0M = ComputeCentrality();
    
    fCentrality = fESD->GetCentrality();
   // centralityV0M = fCentrality->GetCentralityPercentile("V0M");
    centralityV0MnoFlat = fCentrality->GetCentralityPercentile("V0M");
   // lCentrality =  fCentrality->GetCentralityPercentile(fCentralityEstimator.Data());
    ((TH1F*)fOutputList->FindObject("hCentrality"))->Fill(centralityV0M);
    ((TH1F*)fOutputList->FindObject("hCentralityNoFlat"))->Fill(centralityV0MnoFlat);

    
    
    // check  : events are selected by physics selection class
    
    
         Bool_t isSelectedMB =(((AliInputEventHandler*)(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler()))->IsEventSelected() & AliVEvent::kMB);
         if(isSelectedMB) ((TH1F*)fOutputList->FindObject("hEventSelecInfo"))->Fill(1);
        
        Bool_t isSelectedkCentral =(((AliInputEventHandler*)(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler()))->IsEventSelected() & AliVEvent::kCentral);
             if(isSelectedkCentral) ((TH1F*)fOutputList->FindObject("hEventSelecInfo"))->Fill(2);
    
        Bool_t isSelectedkSemiCentral =(((AliInputEventHandler*)(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler()))->IsEventSelected() & (AliVEvent::kSemiCentral));
        if(isSelectedkSemiCentral) ((TH1F*)fOutputList->FindObject("hEventSelecInfo"))->Fill(7);

    
        Bool_t isSelected =(((AliInputEventHandler*)(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler()))->IsEventSelected() && AliVEvent::kMB);
            if(isSelected) ((TH1F*)fOutputList->FindObject("hEventSelecInfo"))->Fill(8);
    
    
    if(!fMCcase){
        if(!isSelected) {cout<<"Event Rejected"<<endl; return;}
    }

    ((TH1F*)fOutputList->FindObject("hCentralitySelected"))->Fill(centralityV0M);
    
    if(centralityV0MnoFlat>40.0 &&centralityV0MnoFlat<100.0)((TH1F*)fOutputList->FindObject("hCentralityNoFlatSelected"))->Fill(centralityV0MnoFlat);
    
    if(centralityV0M<50.0 || centralityV0M>90.0)return;

    ((TH1F*)fOutputList->FindObject("hCentralitySelected5090"))->Fill(centralityV0M);


    
    ///////////////////////////////////////////////////////////
    const AliESDVertex *PrimaryVertexESD;
    
    AliAnalysisManager *man=AliAnalysisManager::GetAnalysisManager();
    AliInputEventHandler* inputHandler = (AliInputEventHandler*) (man->GetInputEventHandler());
    
      // ---- AliPIDResponse ---- //
    fPIDResponse = inputHandler->GetPIDResponse();
    double nSigTPCPID = 3.5;

    
   // TClonesArray *mcArray       = 0x0;
    AliStack    *mcstack        = 0x0;
    TParticle   *MCLamD1esd     = 0x0;
    TParticle   *MCLamD2esd     = 0x0;
    TParticle   *MCLamesd       = 0x0;
    TParticle   *MCXiD2esd      = 0x0;
    TParticle   *MCXiesd        = 0x0;
    TParticle   *MCXiStarD2esd  = 0x0;
    TParticle   *MCXiStaresd    = 0x0;
    
    Double_t px1,py1,pz1, px2,py2,pz2;
    Double_t p1sq,p2sq,e1,e2,angle;
    Double_t dca3d;
    Float_t dca2[2];
    Double_t xiVtx[3];//, xiStarVtx[3];
    Double_t xiP[3], xiStarP[3];
    Double_t xiStarMom;
    Double_t xiMass, xiStarMass;
    Double_t xiPt, xiStarPt;
    Double_t xiY, xiStarY, MCxiStarY;
    Double_t xiCharge;
    Double_t decayLengthXY;
    Double_t pDaughter1[3];
    Double_t pDaughter2[3];
    Double_t xDaughter1[3];
    Double_t xDaughter2[3];
    //
    Double_t bField=0;
    UInt_t status=0;
    Int_t positiveTracks=0, negativeTracks=0;
    Int_t myTracks=0;
    Int_t myxiTracks=0;
    Int_t myMCTracks=0;
    //
    Double_t primaryVtx[3]={0};
    Int_t mBin=0;
    Int_t zBin=0;
    Double_t zStep=2*10/Double_t(fZvertexBins), zStart=-10.;
  
    
    //
    Bool_t mcXiFilled=kFALSE;// So that mctracks are never used uninitialized
    
    if(fMCcase){
        
        if(AliAnalysisManager::GetAnalysisManager()->GetMCtruthEventHandler()){
            if(static_cast<AliMCEventHandler*>(AliAnalysisManager::GetAnalysisManager()->GetMCtruthEventHandler())->MCEvent())
                mcstack = static_cast<AliMCEventHandler*>(AliAnalysisManager::GetAnalysisManager()->GetMCtruthEventHandler())->MCEvent()->Stack();
        }
        
        
    }
    
    
    // ESDs
        
        
        
        ((TH1F*)fOutputList->FindObject("fMultDist1"))->Fill(fESD->GetNumberOfTracks());
        PrimaryVertexESD = fESD->GetPrimaryVertex();
        if(!PrimaryVertexESD) return;
        
       fEventNumber =
        ( ( ((ULong64_t)fESD->GetPeriodNumber() ) << 36 ) |
        ( ((ULong64_t)fESD->GetOrbitNumber () ) << 12 ) |
        ((ULong64_t)fESD->GetBunchCrossNumber() )  );
    

            
        primaryVtx[0]=PrimaryVertexESD->GetX(); primaryVtx[1]=PrimaryVertexESD->GetY(); primaryVtx[2]=PrimaryVertexESD->GetZ();
        ((TH3F*)fOutputList->FindObject("fVertexDist1"))->Fill(primaryVtx[0], primaryVtx[1], primaryVtx[2]);
        
        if(fMCcase){
            /////////////////////////////////////////////////
            // Lam mc input
            /////////////////////////////////////////////////
            for (Int_t it = 0; it < mcstack->GetNprimary(); it++) {
                TParticle *mcInputTrack = (TParticle*)mcstack->Particle(it);
                if (!mcInputTrack) {
                    Error("UserExec", "Could not receive track %d", it);
                    continue;
                }
                             if(mcInputTrack->GetPdgCode() != +kXiCode && mcInputTrack->GetPdgCode() != -kXiCode && mcInputTrack->GetPdgCode() != +kXiStarCode && mcInputTrack->GetPdgCode() != -kXiStarCode) continue;
                
                myMCTracks++;
                
                MCxiStarY = mcInputTrack->Y();
                
                
                
                // Xi
                if(mcInputTrack->GetPdgCode() == +kXiCode) {((TH3F*)fOutputList->FindObject("fMCinputTotalXi1"))->Fill(mcInputTrack->Pt(), centralityV0M, mcInputTrack->GetCalcMass());
                               }
                
                if(mcInputTrack->GetPdgCode() == -kXiCode) {((TH3F*)fOutputList->FindObject("fMCinputTotalXibar1"))->Fill(mcInputTrack->Pt(), centralityV0M, mcInputTrack->GetCalcMass());
                              }
                
                ((TH1F*)fOutputList->FindObject("fXiStarYDistMC"))->Fill(MCxiStarY);
                if(MCxiStarY<-0.5 ||MCxiStarY>0.5) continue; // here selection of rapidity for PbPb analysis
                ((TH1F*)fOutputList->FindObject("fQAXiStarYDistMC"))->Fill(MCxiStarY);
                

                // XiStar
                if(mcInputTrack->GetPdgCode() == +kXiStarCode) {((TH3F*)fOutputList->FindObject("fMCinputTotalXiStar1"))->Fill(mcInputTrack->Pt(), centralityV0M, mcInputTrack->GetCalcMass());
                                  }
                
                if(mcInputTrack->GetPdgCode() == -kXiStarCode) {((TH3F*)fOutputList->FindObject("fMCinputTotalXiStarbar1"))->Fill(mcInputTrack->Pt(),centralityV0M, mcInputTrack->GetCalcMass());
                    
                    
                                 }
                
            }
           
        }
        
    // Vertex systematic study default : 10 , loose : 11 , tight : 9 
        if(fabs(primaryVtx[2]) > 10.) return; // Z-Vertex Cut
    
    
        ((TH1F*)fOutputList->FindObject("fMultDist2"))->Fill(fESD->GetNumberOfTracks());
        
        if(fESD->IsPileupFromSPD()) return; // Reject Pile-up events
        
        ((TH1F*)fOutputList->FindObject("fMultDist3"))->Fill(fESD->GetNumberOfTracks());
        ((TH3F*)fOutputList->FindObject("fVertexDist3"))->Fill(primaryVtx[0], primaryVtx[1], primaryVtx[2]);
        

        
        // multiplicity
        
        if(PrimaryVertexESD->GetNContributors() >= 1) ((TH1F*)fOutputList->FindObject("fMultDist4"))->Fill(fESD->GetNumberOfTracks());
        if(PrimaryVertexESD->GetNContributors() < 1) return; // Enrico cut

        Printf("There are %d tracks in this event", fESD->GetNumberOfTracks());
        
        bField = fESD->GetMagneticField();
        
        
        // Track loop
        for (Int_t i = 0; i < fESD->GetNumberOfTracks(); i++) {
            AliESDtrack* esdtrack = fESD->GetTrack(i);
            if (!esdtrack) continue;
            
            status=esdtrack->GetStatus();
            
            if(!fTrackCut->AcceptTrack(esdtrack)) continue;
            
            Bool_t goodMomentum = esdtrack->GetPxPyPz( fTempStruct[myTracks].fP);
            if(!goodMomentum) continue;
            esdtrack->GetXYZ( fTempStruct[myTracks].fX);
            
            
            //=========checking PID =========//
            //// *** TPC *** ////
            Float_t fTPCPIDmom = esdtrack->GetTPCmomentum();
            Float_t sigTPC = esdtrack->GetTPCsignal();
            Float_t nsigpi= fabs(fPIDResponse->NumberOfSigmasTPC(esdtrack,AliPID::kPion));
            Float_t nsigk= fabs(fPIDResponse->NumberOfSigmasTPC(esdtrack,AliPID::kKaon));
            Float_t nsigpr= fabs(fPIDResponse->NumberOfSigmasTPC(esdtrack,AliPID::kProton));
           
            
            ((TH2F*)fOutputList->FindObject("TPCPID"))->Fill(fTPCPIDmom,sigTPC);
            if(nsigpi<3.) ((TH2F*)fOutputList->FindObject("hTPCPIDpi"))->Fill(fTPCPIDmom,sigTPC);
            if(nsigk<3.) ((TH2F*)fOutputList->FindObject("hTPCPIDk"))->Fill(fTPCPIDmom,sigTPC);
            if(nsigpr<3.) ((TH2F*)fOutputList->FindObject("hTPCPIDp"))->Fill(fTPCPIDmom,sigTPC);
            
            ((TH2F*)fOutputList->FindObject("hNSig3rdPion"))->Fill(fTPCPIDmom,fPIDResponse->NumberOfSigmasTPC(esdtrack,AliPID::kPion));
            
            //=========selecting 3rd pion using PID=========//
 /* TPC OFF */           if(nsigpi>nSigTPCPID) continue; // last update // 20140715 // TPC
            
            
            ((TH2F*)fOutputList->FindObject("hQANSig3rdPion"))->Fill(fTPCPIDmom,fPIDResponse->NumberOfSigmasTPC(esdtrack,AliPID::kPion));

            
            
            esdtrack->GetCovarianceXYZPxPyPz( fTempStruct[myTracks].fCov);
            //esdtrack->GetImpactParameters(dca2, cov);
            dca2[0] = sqrt( pow(fTempStruct[myTracks].fX[0] - primaryVtx[0],2) + pow(fTempStruct[myTracks].fX[1] - primaryVtx[1],2));
            dca2[1] = sqrt( pow(fTempStruct[myTracks].fX[2] - primaryVtx[2],2));
            dca3d = sqrt( pow(dca2[0],2) + pow(dca2[1],2));
            
            ((TH1F*)fOutputList->FindObject("fDCADist"))->Fill(fESD->GetNumberOfTracks(), dca3d);
           // ((TH3F*)fOutputList->FindObject("fPhiPtDist"))->Fill(esdtrack->Charge(), esdtrack->Phi(), esdtrack->Pt());
           // ((TH3F*)fOutputList->FindObject("fPtEtaDist"))->Fill(esdtrack->Charge(), esdtrack->Pt(), esdtrack->Eta());
            
            ((TH1F*)fOutputList->FindObject("fPtDist"))->Fill(esdtrack->Pt());
            ((TH1F*)fOutputList->FindObject("fPhiDist"))->Fill(esdtrack->Phi());
            ((TH1F*)fOutputList->FindObject("fEtaDist"))->Fill(esdtrack->Eta());
            
            
            
            
            
            fTempStruct[myTracks].fStatus = status;
            fTempStruct[myTracks].fID = esdtrack->GetID();
            fTempStruct[myTracks].fLabel = esdtrack->GetLabel();
            fTempStruct[myTracks].fPhi = atan2(fTempStruct[myTracks].fP[1], fTempStruct[myTracks].fP[0]);
            if(fTempStruct[myTracks].fPhi < 0) fTempStruct[myTracks].fPhi += 2*PI;
            fTempStruct[myTracks].fPt = sqrt(pow(fTempStruct[myTracks].fP[0],2) + pow(fTempStruct[myTracks].fP[1],2));
            fTempStruct[myTracks].fMom = sqrt( pow(fTempStruct[myTracks].fPt,2) + pow(fTempStruct[myTracks].fP[2],2) );
            fTempStruct[myTracks].fEta = esdtrack->Eta();
            fTempStruct[myTracks].fCharge = esdtrack->Charge();
            fTempStruct[myTracks].fDCAXY = dca2[0];
            fTempStruct[myTracks].fDCAZ = dca2[1];
            fTempStruct[myTracks].fDCA = dca3d;
            fTempStruct[myTracks].fNSigmaPi = fabs(fPIDResponse->NumberOfSigmasTPC(esdtrack,AliPID::kPion));
            fTempStruct[myTracks].fNSigmaK = fabs(fPIDResponse->NumberOfSigmasTPC(esdtrack,AliPID::kKaon));
            fTempStruct[myTracks].fNSigmaPr = fabs(fPIDResponse->NumberOfSigmasTPC(esdtrack,AliPID::kProton));
            fTempStruct[myTracks].fNclusTPC = esdtrack->GetTPCNcls();
            
            
            if(esdtrack->Charge() > 0) positiveTracks++;
            else negativeTracks++;
            
           if(fTempStruct[myTracks].fNclusTPC < 60) continue;
         //   if(dca2[1]>3) continue;
         //   if(dca2[0]>3) continue;
            
            
            myTracks++;
            
        }
        

    
    
    
    
    
    if(myTracks >= 1) {
        ((TH1F*)fOutputList->FindObject("fMultDist5"))->Fill(myTracks);
        ((TH1F*)fOutputList->FindObject("fMultDist6"))->Fill(fESD->GetNumberOfTracks());
       // ((TH3F*)fOutputList->FindObject("fMultDist3d"))->Fill(positiveTracks+negativeTracks, positiveTracks, negativeTracks);
    }
    
    cout<<"There are "<<myTracks<<"  myTracks"<<endl;
    
    // set Z Vertex bin
    for(Int_t i=0; i<fZvertexBins; i++){
        if( (primaryVtx[2] > zStart+i*zStep) && (primaryVtx[2] < zStart+(i+1)*zStep) ){
            zBin=i;
            break;
        }
    }
    
    // set Multiplicity bin
    for(Int_t i=0; i<fMultBins; i++){
        if( ( myTracks > fMultLimits[i]) && ( myTracks <= fMultLimits[i+1]) ) { mBin=i; break;}
    }
    
  
    ////////////////////////////////////
    // Add event to buffer if > 0 tracks
    if(myTracks > 0){
        fEC[zBin][mBin]->FIFOShift();
        (fEvt) = fEC[zBin][mBin]->fEvtStr;
        (fEvt)->fNTracks = myTracks;
        for(Int_t i=0; i<myTracks; i++) (fEvt)->fTracks[i] = fTempStruct[i];
    }
    
    
    
    if(fMCcase){// get Input MC information for ESD case
        
        /////////////////////////////////////////////////
        // Xi mc input
        /////////////////////////////////////////////////
        for (Int_t it = 0; it < mcstack->GetNprimary(); it++) {
            TParticle *mcInputTrackXi = (TParticle*)mcstack->Particle(it);
            if (!mcInputTrackXi) {
                Error("UserExec", "Could not receive track %d", it);
                continue;
            }
            
            //if(!mcstack->IsPhysicalPrimary(it)) continue;
            if(abs(mcInputTrackXi->GetPdgCode())!=kXiCode) continue;
            
            if(mcInputTrackXi->GetPdgCode() == +kXiCode) {
                ((TH3F*)fOutputList->FindObject("fMCinputTotalXi3"))->Fill(mcInputTrackXi->Pt(), centralityV0M, mcInputTrackXi->GetCalcMass());
            }
            else {((TH3F*)fOutputList->FindObject("fMCinputTotalXibar3"))->Fill(mcInputTrackXi->Pt(), centralityV0M, mcInputTrackXi->GetCalcMass());
               
            }
            
        }
        
        
        /////////////////////////////////////////////////
        // XiStar mc input
        /////////////////////////////////////////////////
        for (Int_t it = 0; it < mcstack->GetNprimary(); it++) {
            TParticle *mcInputTrackXiStar = (TParticle*)mcstack->Particle(it);
            if (!mcInputTrackXiStar) {
                Error("UserExec", "Could not receive track %d", it);
                continue;
            }
            
            if(abs(mcInputTrackXiStar->GetPdgCode())!=kXiStarCode) continue;
            if(mcInputTrackXiStar->Y()<-0.5 ||mcInputTrackXiStar->Y()>0.5) continue; // here selection of rapidity for PbPb analysis

            if(mcInputTrackXiStar->GetPdgCode() == +kXiStarCode) {((TH3F*)fOutputList->FindObject("fMCinputTotalXiStar3"))->Fill(mcInputTrackXiStar->Pt(), centralityV0M, mcInputTrackXiStar->GetCalcMass());
            }
            else {((TH3F*)fOutputList->FindObject("fMCinputTotalXiStarbar3"))->Fill(mcInputTrackXiStar->Pt(), centralityV0M, mcInputTrackXiStar->GetCalcMass());
                
            }
            
            
        }
    }
    
      ////////////////////////////////////////////////
    // Reconstruction
    
    for(Int_t i=0; i<fESD->GetNumberOfCascades(); i++){
        
        AliESDcascade *Xicandidate = fESD->GetCascade(i);
        
        
        if(TMath::Abs( Xicandidate->GetPindex()) == TMath::Abs( Xicandidate->GetNindex())) continue;
        if(TMath::Abs( Xicandidate->GetPindex()) == TMath::Abs( Xicandidate->GetBindex())) continue;
        if(TMath::Abs( Xicandidate->GetNindex()) == TMath::Abs( Xicandidate->GetBindex())) continue;
        

        
        AliESDtrack *pTrackXi	= fESD->GetTrack(TMath::Abs( Xicandidate->GetPindex()));
        AliESDtrack *nTrackXi	= fESD->GetTrack(TMath::Abs( Xicandidate->GetNindex()));
        AliESDtrack *bTrackXi	= fESD->GetTrack(TMath::Abs( Xicandidate->GetBindex()));
        
        // Standard track QA cuts
        if(!fTrackCut->AcceptTrack(pTrackXi)) continue;
        if(!fTrackCut->AcceptTrack(nTrackXi)) continue;
        if(!fTrackCut->AcceptTrack(bTrackXi)) continue;
        

        
        //////////////////////
        // DecayParameters Key (number represents array index)
        // NclustersTPC: 0=proton, 1=pion first, 2=pion second, 3=pion third
        // DCAVtx: 4=proton, 5=pion first, 6=pion second, 7=lambda, 8=pion third
        // 9 = DCA proton-pion
        // 10 = DCA Lambda-pion
        // 11 = Cos PA Lambda
        // 12 = Cos PA Xi
        
         //myxiTracks++;
        
        fDecayParameters[2] = bTrackXi->GetTPCNcls();
        ((TH1F*)fOutputList->FindObject("fTPCNcls_pi2"))->Fill(fDecayParameters[2]);
        
        Double_t fTPCNSigProton = 10;
        Double_t fTPCNSigPion1 = 10;
        Double_t fTPCNSigPion2 = 10;
        
        
        Double_t fTPCPIDMomXi[3] = {-10,-10,-10};
        Double_t fNSigTPCXi[3] = {-10,-10,-10};
        

        
        
        if(Xicandidate->Charge() == -1){
            fDecayParameters[0] = pTrackXi->GetTPCNcls();
            fDecayParameters[1] = nTrackXi->GetTPCNcls();
            fDecayParameters[4] = fabs(pTrackXi->GetD(primaryVtx[0],primaryVtx[1],bField));// DCA Vtx proton
            fDecayParameters[5] = fabs(nTrackXi->GetD(primaryVtx[0],primaryVtx[1],bField));// DCA Vtx pion first
            
            fTPCNSigProton = fPIDResponse->NumberOfSigmasTPC(pTrackXi,AliPID::kProton);
            fTPCNSigPion1 = fPIDResponse->NumberOfSigmasTPC(nTrackXi,AliPID::kPion);
            fTPCNSigPion2 = fPIDResponse->NumberOfSigmasTPC(bTrackXi,AliPID::kPion);
            
            fTPCPIDMomXi[0] = pTrackXi->GetTPCmomentum();
            fNSigTPCXi[0] = pTrackXi->GetTPCsignal();
            
            fTPCPIDMomXi[1] = nTrackXi->GetTPCmomentum();
            fNSigTPCXi[1] = nTrackXi->GetTPCsignal();
            
            fTPCPIDMomXi[2] = bTrackXi->GetTPCmomentum();
            fNSigTPCXi[2] = bTrackXi->GetTPCsignal();
           
        }else{
            fDecayParameters[0] = nTrackXi->GetTPCNcls();
            fDecayParameters[1] = pTrackXi->GetTPCNcls();
            fDecayParameters[4] = fabs(nTrackXi->GetD(primaryVtx[0],primaryVtx[1],bField));// DCA Vtx proton
            fDecayParameters[5] = fabs(pTrackXi->GetD(primaryVtx[0],primaryVtx[1],bField));// DCA Vtx pion first
            
            fTPCNSigProton = fPIDResponse->NumberOfSigmasTPC(nTrackXi,AliPID::kProton);
            fTPCNSigPion1 = fPIDResponse->NumberOfSigmasTPC(pTrackXi,AliPID::kPion);
            fTPCNSigPion2 = fPIDResponse->NumberOfSigmasTPC(bTrackXi,AliPID::kPion);
            
            
            fTPCPIDMomXi[0] = nTrackXi->GetTPCmomentum();
            fNSigTPCXi[0] = nTrackXi->GetTPCsignal();
            
            fTPCPIDMomXi[1] = pTrackXi->GetTPCmomentum();
            fNSigTPCXi[1] = pTrackXi->GetTPCsignal();
            
            fTPCPIDMomXi[2] = bTrackXi->GetTPCmomentum();
            fNSigTPCXi[2] = bTrackXi->GetTPCsignal();
        }
        
        ((TH1F*)fOutputList->FindObject("fTPCNcls_p"))->Fill(fDecayParameters[0]);
        ((TH1F*)fOutputList->FindObject("fTPCNcls_pi1"))->Fill(fDecayParameters[1]);
        ((TH1F*)fOutputList->FindObject("fDCADist_p"))->Fill(fDecayParameters[4]);
        ((TH1F*)fOutputList->FindObject("fDCADist_pi1"))->Fill(fDecayParameters[5]);
        
        ((TH1F*)fOutputList->FindObject("fTPCNSigProton"))->Fill(fTPCNSigProton);
        ((TH1F*)fOutputList->FindObject("fTPCNSigPion1"))->Fill(fTPCNSigPion1);
        ((TH1F*)fOutputList->FindObject("fTPCNSigPion2"))->Fill(fTPCNSigPion2);
        
        ((TH2F*)fOutputList->FindObject("hdEdxProton"))->Fill(fTPCPIDMomXi[0],fNSigTPCXi[0]);
        ((TH2F*)fOutputList->FindObject("hdEdxPion1"))->Fill(fTPCPIDMomXi[1],fNSigTPCXi[1]);
        ((TH2F*)fOutputList->FindObject("hdEdxPion2"))->Fill(fTPCPIDMomXi[2],fNSigTPCXi[2]);
        
        
        if(fTPCNSigProton>-nSigTPCPID&&fTPCNSigProton<nSigTPCPID)((TH2F*)fOutputList->FindObject("hdEdxProtonAfter"))->Fill(fTPCPIDMomXi[0],fNSigTPCXi[0]);
        if(fTPCNSigPion1>-nSigTPCPID&& fTPCNSigPion1<nSigTPCPID) ((TH2F*)fOutputList->FindObject("hdEdxPion1After"))->Fill(fTPCPIDMomXi[1],fNSigTPCXi[1]);
        if(fTPCNSigPion2>-nSigTPCPID&&fTPCNSigPion2<nSigTPCPID)((TH2F*)fOutputList->FindObject("hdEdxPion2After"))->Fill(fTPCPIDMomXi[2],fNSigTPCXi[2]);
        
    
 /* TPC PID OFF */       if(fTPCNSigProton<-nSigTPCPID||fTPCNSigProton>nSigTPCPID) continue; // PID for proton
 /* TPC PID OFF */      if(fTPCNSigPion1<-nSigTPCPID||fTPCNSigPion1>nSigTPCPID) continue; // PID for 1st pion
 /* TPC PID OFF */      if(fTPCNSigPion2<-nSigTPCPID||fTPCNSigPion2>nSigTPCPID) continue; // PID for 2nd pion
        
        ((TH1F*)fOutputList->FindObject("fQATPCNSigProton"))->Fill(fTPCNSigProton);
        ((TH1F*)fOutputList->FindObject("fQATPCNSigPion1"))->Fill(fTPCNSigPion1);
        ((TH1F*)fOutputList->FindObject("fQATPCNSigPion2"))->Fill(fTPCNSigPion2);
        
        
        fDecayParameters[6] = fabs(bTrackXi->GetD(primaryVtx[0],primaryVtx[1],bField));// DCA Vtx pion second
        ((TH1F*)fOutputList->FindObject("fDCADist_pi2"))->Fill(fDecayParameters[6]);
        fDecayParameters[7] = fabs(Xicandidate->GetD(primaryVtx[0],primaryVtx[1],primaryVtx[2]));// DCA Vtx Lambda
        ((TH1F*)fOutputList->FindObject("fDCADist_lambda"))->Fill(fDecayParameters[7]);
        fDecayParameters[9] = fabs(Xicandidate->GetDcaV0Daughters());// DCA proton-pion
        ((TH1F*)fOutputList->FindObject("fDCADist_pi_p"))->Fill(fDecayParameters[9]);
        fDecayParameters[10] = fabs(Xicandidate->GetDcaXiDaughters());// DCA Lambda-pion
        ((TH1F*)fOutputList->FindObject("fDCADist_pi_lambda"))->Fill(fDecayParameters[10]);
        
        Double_t tempX[3]={0};
        Xicandidate->GetXYZ(tempX[0], tempX[1], tempX[2]);
        
    //    fDecayParameters[11] = sqrt( pow(tempX[0],2) + pow(tempX[1],2));// Rxy Lambda
    //    ((TH1F*)fOutputList->FindObject("fRXY_lambda"))->Fill(fDecayParameters[11]);
        
        fDecayParameters[11] = Xicandidate->GetV0CosineOfPointingAngle(primaryVtx[0],primaryVtx[1],primaryVtx[2]);// Cos PA Lambda
        ((TH1F*)fOutputList->FindObject("fCosPA_lambda"))->Fill(fDecayParameters[11]);
        
        fDecayParameters[12] = Xicandidate->GetCascadeCosineOfPointingAngle(primaryVtx[0],primaryVtx[1],primaryVtx[2]);// Cos PA Xi
        ((TH1F*)fOutputList->FindObject("fCosPA_Xi"))->Fill(fDecayParameters[12]);
        
        decayLengthXY = sqrt( pow(xiVtx[0]-primaryVtx[0],2) + pow(xiVtx[1]-primaryVtx[1],2) );
   //     fDecayParameters[12] = decayLengthXY;// Rxy Xi
   //     ((TH1F*)fOutputList->FindObject("fRXY_Xi"))->Fill(fDecayParameters[12]);
        
        
        xiP[0] = Xicandidate->Px();
        xiP[1] = Xicandidate->Py();
        xiP[2] = Xicandidate->Pz();
        xiVtx[0] = Xicandidate->Xv();
        xiVtx[1] = Xicandidate->Yv();
        xiVtx[2] = Xicandidate->Zv();
        xiPt = Xicandidate->Pt();
        xiY = Xicandidate->RapXi();
        xiMass = Xicandidate->M();
        xiCharge = Xicandidate->Charge();
 
        myxiTracks++;

        
        
        if(sqrt( pow(tempX[0],2) + pow(tempX[1],2) ) > fMaxDecayLength) continue;
        if(decayLengthXY > fMaxDecayLength) continue;
        
        Bool_t StandardXi=kTRUE;
        if(StandardXi)((TH1F*)fOutputList->FindObject("fCutEvents"))->Fill(1,1);
        
        if(fDecayParameters[0] < fCutValues[0][0]) StandardXi=kFALSE;// Nclus proton
        if(StandardXi)((TH1F*)fOutputList->FindObject("fCutEvents"))->Fill(2,1);
        
        if(fDecayParameters[1] < fCutValues[0][1]) StandardXi=kFALSE;// Nclus pion first
        if(StandardXi)((TH1F*)fOutputList->FindObject("fCutEvents"))->Fill(3,1);
        
        if(fDecayParameters[2] < fCutValues[0][2]) StandardXi=kFALSE;// Nclus pion second
        if(StandardXi)((TH1F*)fOutputList->FindObject("fCutEvents"))->Fill(4,1);
        
        //
        if(fDecayParameters[4] < fCutValues[0][4]) StandardXi=kFALSE;// DCAVtx proton
        if(StandardXi)((TH1F*)fOutputList->FindObject("fCutEvents"))->Fill(5,1);
        
        if(fDecayParameters[5] < fCutValues[0][5]) StandardXi=kFALSE;// DCAVtx pion first
        if(StandardXi)((TH1F*)fOutputList->FindObject("fCutEvents"))->Fill(6,1);
        
        if(fDecayParameters[6] < fCutValues[0][6]) StandardXi=kFALSE;// DCAVtx pion second
        if(StandardXi)((TH1F*)fOutputList->FindObject("fCutEvents"))->Fill(7,1);
        
        if(fDecayParameters[7] < fCutValues[0][7]) StandardXi=kFALSE;// DCAVtx Lambda
        if(StandardXi)((TH1F*)fOutputList->FindObject("fCutEvents"))->Fill(8,1);
        
        //
        if(fDecayParameters[9] > fCutValues[0][9]) StandardXi=kFALSE;// DCAV proton-pion
        if(StandardXi)((TH1F*)fOutputList->FindObject("fCutEvents"))->Fill(9,1);
        
        if(fDecayParameters[10] > fCutValues[0][10]) StandardXi=kFALSE;// DCAV Lambda-pion
        if(StandardXi)((TH1F*)fOutputList->FindObject("fCutEvents"))->Fill(10,1);
        
        //
       // if(fDecayParameters[11] < fCutValues[0][11]) StandardXi=kFALSE;// Rxy Lambda
       // if(StandardXi)((TH1F*)fOutputList->FindObject("fCutEvents"))->Fill(11,1);
        
       // if(fDecayParameters[12] < fCutValues[0][12]) StandardXi=kFALSE;// Rxy Xi
       // if(StandardXi)((TH1F*)fOutputList->FindObject("fCutEvents"))->Fill(12,1);
        
        //
        if(fDecayParameters[11] < fCutValues[0][11]) StandardXi=kFALSE;// Cos PA Lambda
        if(StandardXi)((TH1F*)fOutputList->FindObject("fCutEvents"))->Fill(11,1);
        
        if(fDecayParameters[12] < fCutValues[0][12]) StandardXi=kFALSE;// Cos PA Xi
        if(StandardXi)((TH1F*)fOutputList->FindObject("fCutEvents"))->Fill(12,1);
        
    
        if(StandardXi){
            if(xiCharge == -1) {
                CutVar[0].fXi->Fill(xiPt, centralityV0M, xiMass);
                ((TH1F*)fOutputList->FindObject("hXiInvMass"))->Fill(xiMass);
            }
            else {
                CutVar[0].fXibar->Fill(xiPt, centralityV0M, xiMass);
                ((TH1F*)fOutputList->FindObject("hXiInvMass"))->Fill(xiMass);
            }
        }
     

        
        
        // MC associaton
        mcXiFilled = kFALSE;
        if(fMCcase ){
            
            MCXiD2esd = (TParticle*)mcstack->Particle(abs(bTrackXi->GetLabel()));
            
            if(abs(MCXiD2esd->GetPdgCode())==kPionCode){
                
                MCLamD1esd = (TParticle*)mcstack->Particle(abs(pTrackXi->GetLabel()));
                MCLamD2esd = (TParticle*)mcstack->Particle(abs(nTrackXi->GetLabel()));
                
                if(MCLamD1esd->GetMother(0) == MCLamD2esd->GetMother(0)){
                    if(abs(MCLamD1esd->GetPdgCode())==kProtonCode || abs(MCLamD2esd->GetPdgCode())==kProtonCode) {
                        if(abs(MCLamD1esd->GetPdgCode())==kPionCode || abs(MCLamD2esd->GetPdgCode())==kPionCode) {
                            
                            MCLamesd = (TParticle*)mcstack->Particle(abs(MCLamD1esd->GetMother(0)));
                            if(abs(MCLamesd->GetPdgCode())==kLambdaCode) {
                                
                                if(MCLamesd->GetMother(0) == MCXiD2esd->GetMother(0)){
                                    MCXiesd = (TParticle*)mcstack->Particle(abs(MCLamesd->GetMother(0)));
                                    if(abs(MCXiesd->GetPdgCode())==kXiCode) {
                                        mcXiFilled = kTRUE;

                                        if(StandardXi){
                                            if(Xicandidate->Charge() == -1){
                                                
                                                CutVar[0].fMCrecXi->Fill(xiPt, centralityV0M, xiMass);
                                            }else{
                                                CutVar[0].fMCrecXibar->Fill(xiPt, centralityV0M, xiMass);
                                            }
                                            
                                            
                                        }
                                        
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }// MC association
        
        
        if(fabs(xiMass-fTrueMassXi) > fMassWindow) continue;
        
        if(StandardXi)((TH1F*)fOutputList->FindObject("hQAXiInvMass"))->Fill(xiMass);
        
        fXiTrack->Set(xiVtx, xiP, fCovMatrix, Short_t(xiCharge));
        
        
        
        //////////////////////////////////////////////////////////
        // Reconstruct Xi(1530)
        for(Int_t EN=0; EN<fEventsToMix+1; EN++){// Event buffer loop
            
            for(Int_t l=0; l<(fEvt+EN)->fNTracks; l++){// Present(EN=0) and Past(EN from 1 to fEventsToMix) event track loop
                
                if(EN==0) {
                    if((fEvt+EN)->fTracks[l].fID == pTrackXi->GetID()) continue;
                    if((fEvt+EN)->fTracks[l].fID == nTrackXi->GetID()) continue;
                    if((fEvt+EN)->fTracks[l].fID == bTrackXi->GetID()) continue;
                }
                
                fXiTrack->Set(xiVtx, xiP, fCovMatrix, Short_t(xiCharge));
                
                if(!fESDTrack4) continue;
                fESDTrack4->Set((fEvt+EN)->fTracks[l].fX, (fEvt+EN)->fTracks[l].fP, (fEvt+EN)->fTracks[l].fCov, (fEvt+EN)->fTracks[l].fCharge);
  
                    fDecayParameters[8] = (fEvt+EN)->fTracks[l].fDCAXY;// DCA Vtx pion third
                    ((TH1F*)fOutputList->FindObject("fDCADist_3rd_pi"))->Fill(fDecayParameters[8]);
                    
                    
                
                    if((fEvt+EN)->fTracks[l].fDCAZ > 2) continue;
                    if( (((fEvt+EN)->fTracks[l].fStatus)&AliESDtrack::kITSrefit)==0) continue;// Require itsrefit
                    // no Chi^2 cut applied for ESDs.  Info not available in my track structure.
            
                
                if(fabs((fEvt+EN)->fTracks[l].fEta) > 0.8) continue;
                
                fDecayParameters[3] = (fEvt+EN)->fTracks[l].fNclusTPC;
                ((TH1F*)fOutputList->FindObject("fTPCNcls_pi3"))->Fill(fDecayParameters[3]);
                
                AliVertex *XiStarVtx = new AliVertex((fEvt+EN)->fTracks[l].fX,0,0);
                //fESDTrack4->PropagateToDCA(fXiTrack, bField);// Propagate tracks to dca, both tracks are budged
                if(!(fXiTrack->PropagateToDCA(XiStarVtx, bField, 3))) continue;// Propagate tracks to dca, version which assumes fESDTrack4 is already primary
                /////////////
                fXiTrack->GetPxPyPz(pDaughter1);
                fXiTrack->GetXYZ(xDaughter1);
                fESDTrack4->GetPxPyPz(pDaughter2);
                fESDTrack4->GetXYZ(xDaughter2);
                //////////////////////////
                
                
                
                //xiStarVtx[0] = (xDaughter1[0]+xDaughter2[0])/2.;
                //xiStarVtx[1] = (xDaughter1[1]+xDaughter2[1])/2.;
                //xiStarVtx[2] = (xDaughter1[2]+xDaughter2[2])/2.;
                //decayLength = sqrt(pow(xiStarVtx[0]-primaryVtx[0],2)+pow(xiStarVtx[1]-primaryVtx[1],2)+pow(xiStarVtx[2]-primaryVtx[2],2));
                
                px1=pDaughter1[0];
                py1=pDaughter1[1];
                pz1=pDaughter1[2];
                px2=pDaughter2[0];
                py2=pDaughter2[1];
                pz2=pDaughter2[2];
                
                p1sq=px1*px1+py1*py1+pz1*pz1;
                p2sq=px2*px2+py2*py2+pz2*pz2;
                if(p1sq <=0 || p2sq <=0) continue;
                
                e1=sqrt(p1sq+fTrueMassXi*fTrueMassXi);
                e2=sqrt(p2sq+fTrueMassPi*fTrueMassPi);
                angle=px1*px2+py1*py2+pz1*pz2;
                xiStarMass=fTrueMassXi*fTrueMassXi+fTrueMassPi*fTrueMassPi+2.*e1*e2-2.*angle;
                if(xiStarMass<0.) xiStarMass=1.e-8;
                xiStarMass=sqrt(xiStarMass);
                
                
                xiStarP[0] = px1+px2;
                xiStarP[1] = py1+py2;
                xiStarP[2] = pz1+pz2;
                xiStarMom = sqrt(pow(xiStarP[0],2)+pow(xiStarP[1],2)+pow(xiStarP[2],2));
                if(xiStarMom==0) continue; // So one of the following lines doesnt break
                xiStarPt = sqrt(xiStarP[0]*xiStarP[0] + xiStarP[1]*xiStarP[1]);
                
                
                xiStarY = .5*log( ((e1+e2) + xiStarP[2])/((e1+e2) - xiStarP[2]));
                                ((TH1F*)fOutputList->FindObject("fXiStarYDist"))->Fill(xiStarY);
                if(xiStarY<-0.5 ||xiStarY>0.5) continue; // here selection of rapidity for PbPb analysis

                               ((TH1F*)fOutputList->FindObject("fQAXiStarYDist"))->Fill(xiStarY);
                
                for(int cv=0; cv<kNCutVariations; cv++){
                    
                    
                    if(fDecayParameters[0] < fCutValues[cv][0]) continue;// Nclus proton
                    if(fDecayParameters[1] < fCutValues[cv][1]) continue;// Nclus pion first
                    if(fDecayParameters[2] < fCutValues[cv][2]) continue;// Nclus pion second
                    if(fDecayParameters[3] < fCutValues[cv][3]) continue;// Nclus pion third
                    //
                    if(fDecayParameters[4] < fCutValues[cv][4]) continue;// DCAVtx proton
                    if(fDecayParameters[5] < fCutValues[cv][5]) continue;// DCAVtx pion first
                    if(fDecayParameters[6] < fCutValues[cv][6]) continue;// DCAVtx pion second
                    if(fDecayParameters[7] < fCutValues[cv][7]) continue;// DCAVtx Lambda
                    if(fDecayParameters[8] > fCutValues[cv][8]) continue; // DCAVtx pion third
                    //if(cv!=8 || cv!=21) {if(fDecayParameters[8] > (0.0105 + 0.035/pow((fEvt+EN)->fTracks[l].fPt,1.01))) continue;}// DCAVtx pion third
                    //0.0182 + 0.035/pow((fEvt+EN)->fTracks[l].fPt,1.01
                    
                    //
                    if(fDecayParameters[9] > fCutValues[cv][9]) continue;// DCAV proton-pion
                    if(fDecayParameters[10] > fCutValues[cv][10]) continue;// DCAV Lambda-pion
                    //
                    if(fDecayParameters[11] < fCutValues[cv][11]) continue;// Cos PA Lambda
                    if(fDecayParameters[12] < fCutValues[cv][12]) continue;// Cos PA Xi
                    if(EN==0 && cv==0){  // cut QA plot for default cut
                        ((TH1F*)fOutputList->FindObject("fQATPCNcls_p"))->Fill(fDecayParameters[0]);
                        ((TH1F*)fOutputList->FindObject("fQATPCNcls_pi1"))->Fill(fDecayParameters[1]);
                        ((TH1F*)fOutputList->FindObject("fQATPCNcls_pi2"))->Fill(fDecayParameters[2]);
                        ((TH1F*)fOutputList->FindObject("fQATPCNcls_pi3"))->Fill(fDecayParameters[3]);
                        
                        ((TH1F*)fOutputList->FindObject("fQADCADist_p"))->Fill(fDecayParameters[4]);
                        ((TH1F*)fOutputList->FindObject("fQADCADist_pi1"))->Fill(fDecayParameters[5]);
                        ((TH1F*)fOutputList->FindObject("fQADCADist_pi2"))->Fill(fDecayParameters[6]);
                        
                        ((TH1F*)fOutputList->FindObject("fQADCADist_lambda"))->Fill(fDecayParameters[7]);
                        ((TH1F*)fOutputList->FindObject("fQADCADist_3rd_pi"))->Fill(fDecayParameters[8]);
                        ((TH1F*)fOutputList->FindObject("fQADCADist_pi_p"))->Fill(fDecayParameters[9]);
                        ((TH1F*)fOutputList->FindObject("fQADCADist_pi_lambda"))->Fill(fDecayParameters[10]);
                        ((TH1F*)fOutputList->FindObject("fQACosPA_lambda"))->Fill(fDecayParameters[11]);
                        ((TH1F*)fOutputList->FindObject("fQACosPA_Xi"))->Fill(fDecayParameters[12]);
                    }
                    
                    if(EN==0 && cv==1){
                        ((TH1F*)fOutputList->FindObject("fQATPCNcls_p_L"))->Fill(fDecayParameters[0]);
                        ((TH1F*)fOutputList->FindObject("fQATPCNcls_pi1_L"))->Fill(fDecayParameters[1]);
                        ((TH1F*)fOutputList->FindObject("fQATPCNcls_pi2_L"))->Fill(fDecayParameters[2]);
                        ((TH1F*)fOutputList->FindObject("fQATPCNcls_pi3_L"))->Fill(fDecayParameters[3]);
                    }
                    if(EN==0){
                        
                        if(cv==2)((TH1F*)fOutputList->FindObject("fQADCADist_p_L"))->Fill(fDecayParameters[4]);
                        if(cv==3)((TH1F*)fOutputList->FindObject("fQADCADist_pi1_L"))->Fill(fDecayParameters[5]);
                        if(cv==4)((TH1F*)fOutputList->FindObject("fQADCADist_pi2_L"))->Fill(fDecayParameters[6]);
                        if(cv==5)((TH1F*)fOutputList->FindObject("fQADCADist_lambda_L"))->Fill(fDecayParameters[7]);
                        if(cv==6)((TH1F*)fOutputList->FindObject("fQADCADist_3rd_pi_L"))->Fill(fDecayParameters[8]);
                        if(cv==7)((TH1F*)fOutputList->FindObject("fQADCADist_pi_p_L"))->Fill(fDecayParameters[9]);
                        if(cv==8)((TH1F*)fOutputList->FindObject("fQADCADist_pi_lambda_L"))->Fill(fDecayParameters[10]);
                        if(cv==9)((TH1F*)fOutputList->FindObject("fQACosPA_lambda_L"))->Fill(fDecayParameters[11]);
                        if(cv==10)((TH1F*)fOutputList->FindObject("fQACosPA_Xi_L"))->Fill(fDecayParameters[12]);

                    }
                    
                    if(EN==0 && cv==11){
                        ((TH1F*)fOutputList->FindObject("fQATPCNcls_p_T"))->Fill(fDecayParameters[0]);
                        ((TH1F*)fOutputList->FindObject("fQATPCNcls_pi1_T"))->Fill(fDecayParameters[1]);
                        ((TH1F*)fOutputList->FindObject("fQATPCNcls_pi2_T"))->Fill(fDecayParameters[2]);
                        ((TH1F*)fOutputList->FindObject("fQATPCNcls_pi3_T"))->Fill(fDecayParameters[3]);
                    }
                    if(EN==0){
                        
                        if(cv==12)((TH1F*)fOutputList->FindObject("fQADCADist_p_T"))->Fill(fDecayParameters[4]);
                        if(cv==13)((TH1F*)fOutputList->FindObject("fQADCADist_pi1_T"))->Fill(fDecayParameters[5]);
                        if(cv==14)((TH1F*)fOutputList->FindObject("fQADCADist_pi2_T"))->Fill(fDecayParameters[6]);
                        if(cv==15)((TH1F*)fOutputList->FindObject("fQADCADist_lambda_T"))->Fill(fDecayParameters[7]);
                        if(cv==16)((TH1F*)fOutputList->FindObject("fQADCADist_3rd_pi_T"))->Fill(fDecayParameters[8]);
                        if(cv==17)((TH1F*)fOutputList->FindObject("fQADCADist_pi_p_T"))->Fill(fDecayParameters[9]);
                        if(cv==18)((TH1F*)fOutputList->FindObject("fQADCADist_pi_lambda_T"))->Fill(fDecayParameters[10]);
                        if(cv==19)((TH1F*)fOutputList->FindObject("fQACosPA_lambda_T"))->Fill(fDecayParameters[11]);
                        if(cv==20)((TH1F*)fOutputList->FindObject("fQACosPA_Xi_T"))->Fill(fDecayParameters[12]);
                        

                    }

                    

                    
                    
                    if(EN==0){
                        if(fXiTrack->Charge() == -1 &&  fESDTrack4->Charge() == -1) CutVar[cv].fXiMinusPiMinus->Fill(xiStarPt, centralityV0M, xiStarMass);
                        else if(fXiTrack->Charge() == -1 &&  fESDTrack4->Charge() == +1) {
                            CutVar[cv].fXiMinusPiPlus->Fill(xiStarPt, centralityV0M, xiStarMass);
                        }
                        else if(fXiTrack->Charge() == +1 &&  fESDTrack4->Charge() == -1) {
                            CutVar[cv].fXiPlusPiMinus->Fill(xiStarPt, centralityV0M, xiStarMass);
                        }
                        else CutVar[cv].fXiPlusPiPlus->Fill(xiStarPt, centralityV0M, xiStarMass);
                    }else {
                        if(fXiTrack->Charge() == -1 &&  fESDTrack4->Charge() == -1) CutVar[cv].fXiMinusPiMinusbkg->Fill(xiStarPt, centralityV0M, xiStarMass);
                        else if(fXiTrack->Charge() == -1 &&  fESDTrack4->Charge() == +1) CutVar[cv].fXiMinusPiPlusbkg->Fill(xiStarPt, centralityV0M, xiStarMass);
                        else if(fXiTrack->Charge() == +1 &&  fESDTrack4->Charge() == -1) CutVar[cv].fXiPlusPiMinusbkg->Fill(xiStarPt, centralityV0M, xiStarMass);
                        else CutVar[cv].fXiPlusPiPlusbkg->Fill(xiStarPt, centralityV0M, xiStarMass);
                    }
                    
                    
                    
                                      // MC associaton ESD
                    
                    
                    if(fMCcase && mcXiFilled && EN==0){// ESD MC's
                        MCXiStarD2esd = (TParticle*)mcstack->Particle(abs((fEvt)->fTracks[l].fLabel));
                        
                        if(abs(MCXiStarD2esd->GetPdgCode())==kPionCode){
                            if(MCXiesd->GetMother(0) == MCXiStarD2esd->GetMother(0)){
                                
                                MCXiStaresd = (TParticle*)mcstack->Particle(abs(MCXiesd->GetMother(0)));
                                if(abs(MCXiStaresd->GetPdgCode())==kXiStarCode) {
                                    ((TH1F*)fOutputList->FindObject("fXiStarYDistMCout"))->Fill(xiStarY);
                                    if(fXiTrack->Charge() == -1 &&  fESDTrack4->Charge() == +1) CutVar[cv].fMCrecXiMinusPiPlus->Fill(xiStarPt, centralityV0M, xiStarMass);
                                    if(fXiTrack->Charge() == +1 &&  fESDTrack4->Charge() == -1) CutVar[cv].fMCrecXiPlusPiMinus->Fill(xiStarPt, centralityV0M, xiStarMass);
                                    
                                }
                            }
                        }
                    }
                    
                    
                }// Cut Variation loop
            }// 3rd pion loop
            
            
        }// Event mixing loop
        
        
        
        
        
        
        
    }// Xi loop
    
    
    
    // Post output data.
    PostData(1, fOutputList);
  
    
    
}
//________________________________________________________________________
void AliXiStarPbPb::Terminate(Option_t *)
{
    cout<<"Done"<<endl;
}
//________________________________________________________________________
/*
Double_t AliXiStarPbPb::LinearPropagateToDCA(AliESDtrack *v, AliESDtrack *t, Double_t b) {// Adapted from AliCascadeVertexer.cxx
    //--------------------------------------------------------------------
    // This function returns the DCA between the V0 and the track
    //--------------------------------------------------------------------
    
    Double_t alpha=t->GetAlpha(), cs1=TMath::Cos(alpha), sn1=TMath::Sin(alpha);
    Double_t r[3]; t->GetXYZ(r);
    Double_t x1=r[0], y1=r[1], z1=r[2];
    Double_t p[3]; t->GetPxPyPz(p);
    Double_t px1=p[0], py1=p[1], pz1=p[2];
    
    Double_t x2[3]={0};
    Double_t p2[3]={0};
    Double_t vx2,vy2,vz2;     // position and momentum of V0
    Double_t px2,py2,pz2;
    
    v->GetXYZ(x2);
    v->GetPxPyPz(p2);
    vx2=x2[0], vy2=x2[1], vz2=x2[2];
    px2=p2[0], py2=p2[1], pz2=p2[2];
    
    // calculation dca
    
    Double_t dd= Det(vx2-x1,vy2-y1,vz2-z1,px1,py1,pz1,px2,py2,pz2);
    Double_t ax= Det(py1,pz1,py2,pz2);
    Double_t ay=-Det(px1,pz1,px2,pz2);
    Double_t az= Det(px1,py1,px2,py2);
    
    Double_t dca=TMath::Abs(dd)/TMath::Sqrt(ax*ax + ay*ay + az*az);
    
    //points of the DCA
    Double_t t1 = Det(vx2-x1,vy2-y1,vz2-z1,px2,py2,pz2,ax,ay,az)/
    Det(px1,py1,pz1,px2,py2,pz2,ax,ay,az);
    
    x1 += px1*t1; y1 += py1*t1; //z1 += pz1*t1;
    
    
    //propagate track to the points of DCA
    
    x1=x1*cs1 + y1*sn1;
    if (!t->PropagateTo(x1,b)) {
        Error("PropagateToDCA","Propagation failed !");
        return 1.e+33;
    }
    
    return dca;
}
*/

//________________________________________________________________________
Double_t AliXiStarPbPb::Det(Double_t a00, Double_t a01, Double_t a10, Double_t a11) const {// Taken from AliCascadeVertexer
    //--------------------------------------------------------------------
    // This function calculates locally a 2x2 determinant
    //--------------------------------------------------------------------
    return a00*a11 - a01*a10;
}
//________________________________________________________________________
Double_t AliXiStarPbPb::Det(Double_t a00,Double_t a01,Double_t a02,
                        Double_t a10,Double_t a11,Double_t a12,
                        Double_t a20,Double_t a21,Double_t a22) const {// Taken from AliCascadeVertexer
    //--------------------------------------------------------------------
    // This function calculates locally a 3x3 determinant
    //--------------------------------------------------------------------
    return  a00*Det(a11,a12,a21,a22)-a01*Det(a10,a12,a20,a22)+a02*Det(a10,a11,a20,a21);
}
//________________________________________________________________________

Double_t AliXiStarPbPb::ApplyCentralityPatchPbPb2011(){
    //This part rejects randomly events such that the centrality gets flat for LHC11h Pb-Pb data
    //for 0-5% and 10-20% centrality bin
    
    
    AliCentrality *centrality = fESD->GetCentrality();
    if (!centrality) {
        AliWarning("Cannot get centrality from ESD event.");
        return -999.0;
    }
    
    Double_t cent = (Float_t)(centrality->GetCentralityPercentile("V0M"));
    Double_t rnd_hc = -1., testf = 0.0, ff = 0, N1 = -1., N2 = -1.;
    Int_t fUseCentralityPatchPbPb2011 = 0.;
    fUseCentralityPatchPbPb2011 = 1020;
    
    if(fUseCentralityPatchPbPb2011==510){
        N1 = 1.9404e+06;
        N2 = 1.56435e+06; //N2 is the reference
        ff = 5.04167e+06 - 1.49885e+06*cent + 2.35998e+05*cent*cent -1.22873e+04*cent*cent*cent;
    } else {
    if(fUseCentralityPatchPbPb2011==1020){
            N2 = 2.0e+05; //N2 is the reference
            N1 = 3.7e+05;
            ff = -1.73979e+06 - 3.05316e+06*cent + 1.05517e+06*cent*cent - 133205*cent*cent*cent + 8187.45*cent*cent*cent*cent - 247.875*cent*cent*cent*cent*cent + 2.9676*cent*cent*cent*cent*cent*cent;
        } else {
            AliError(Form("Patch for the requested centrality (%i) is not available", fUseCentralityPatchPbPb2011));
            return -999.0;
        }
    }
    testf = ( N2 + (N1-ff) ) / N1;
    rnd_hc = gRandom->Rndm();
    
    //AliDebugClass(1, Form("Flat Centrality %d", fUseCentralityPatchPbPb2011));
    
    if (rnd_hc < 0 || rnd_hc > 1 )
    {
        AliWarning("Wrong Random number generated");
        return -999.0;
    }
    
    if (rnd_hc < testf)
        return cent;
    else
        return -999.0;
}
//---------------------------------------------------------------------
Double_t AliXiStarPbPb::ComputeCentrality()
{
    //
    // Computes event centrality/multiplicity according to the criterion defined
    // by two elements: (1) choice between multiplicity and centrality and
    // (2) the string defining what criterion must be used for specific computation.
    //
    
    AliCentrality *centrality = fESD->GetCentrality();
    if (!centrality) {
        AliError("Cannot compute centrality!");
        return -1.0;
    }
    
    double SelecCent= 0.;
    SelecCent=centrality->GetCentralityPercentile("V0M");
    
    //if (!fMCcase){
      //  if(SelecCent>90. && SelecCent<100.) // now No flattning
    
       //     return ApplyCentralityPatchPbPb2011();
       // else{
       //     return centrality->GetCentralityPercentile("V0M");
       //     }
       // }
   // if(fMCcase){
       return centrality->GetCentralityPercentile("V0M");
   //     }
    
}




