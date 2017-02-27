//#include "AnalTree.h"
//#include "AliVEvent.h"

AnalTest(){
    for (Int_t k=2; k<3; k++ ) {
        Anal(k);
    }
}

Anal(Int_t split=-1){
    //Int_t mypions;
    
    TStopwatch timer;
    timer.Start();
    // TFile *f = new TFile("MyOutput.root","READ");
    // cout << "open root file " << endl;
    
    
    
    TString ss = Form("%03d",split); //d -> 03d
    cout << ss << endl;
    
    
    TFile *f = TFile::Open(ss+"/MyOutput.root","READ");
    if(f==NULL) continue; // this is for empty directory
    
    cout << "open root file : " << ss << "/MyOutput.root"<< endl;
    
    Int_t nc = 0;
    Int_t EN = 0;
    Int_t ct = 0;
    
    
    const Int_t    kNCutVariations      = 13;// number of cut variations
    const Int_t    kNCuts               = 15; // number of cut types
    
    
    Float_t fTreeVariableMult;
    Float_t fTreeVariablePrimaryVz;
    Float_t fTreeVariableXiMass;
    Float_t fTreeVariableLambdaMass;
    Float_t fTreeVariableXiPt;
    Float_t fTreeVariableEta;
    Float_t fTreeVariablePhi;
    Float_t fTreeVariableDecayXYxi;
    Float_t fTreeVariableDecayXYlambda; // 1-9
    
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
    Float_t fTreevariableCosPAxi; // 14-23
    
    Float_t fTreeVariableCentrality;
    Float_t fTreeVariableCentralityV0M;
    Float_t fTreeVariableCentralityTRK;
    Float_t fTreeVariableCentralitySPD;
    
    Int_t fTreeVariablemyTracks;
    Int_t fTreeVariablexiTracks;
    
    Int_t totmyTracks = 0;
    Int_t totxiTracks = 0;
    
    Float_t fTreeVariableXiTrackX;
    Float_t fTreeVariableXiTrackY;
    Float_t fTreeVariableXiTrackZ;
    Float_t fTreeVariableXiTrackPx;
    Float_t fTreeVariableXiTrackPy;
    Float_t fTreeVariableXiTrackPz;
    Float_t fTreeVariableXiTrackQ;
    Float_t fTreeVariableXiTrackpID;
    Float_t fTreeVariableXiTracknID;
    Float_t fTreeVariableXiTrackbID;
    
    
    Float_t fTreeVariableESDTrackX;
    Float_t fTreeVariableESDTrackY;
    Float_t fTreeVariableESDTrackZ;
    Float_t fTreeVariableESDTrackPx;
    Float_t fTreeVariableESDTrackPy;
    Float_t fTreeVariableESDTrackPz;
    Float_t fTreeVariableESDTrackQ;
    Float_t fTreeVariableESDTrackID;
    
    Float_t fTreeVariavleESDStatus;
    
    Float_t   fTreeVariableESDtrackDCAZ;
    Float_t   fTreeVariableESDtrackDCAXY;
    Float_t   fTreeVariableESDtrackEta;
    Float_t   fTreeVariableESDtrackPt;
    Float_t   fTreeVariableESDtrackMom;
    Float_t   fTreeVariableESDtrackfNclusTPC;
    
    
    
   // Float_t fTreeVariableEventCounter1;
   // Float_t fTreeVariableEventCounter2;
   // Float_t fTreeVariableEventCounter3;
    Int_t fTreeVariableRunNumber;
    ULong64_t fTreeVariableEventNumber1;
    ULong64_t fTreeVariableEventNumber2;
    ULong64_t fTreeVariableEventNumber3;
    
    Float_t fTreeVariableEventNumberInFile;
    
    
    // MC
    
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
    

    
    
    
    Float_t fCutValues[kNCutVariations][kNCuts];// array of reconstruction kinematics
    
    // Set Standard Reconstruction cut values
    fCutValues[0][0] = 70; fCutValues[0][1] = 70; fCutValues[0][2] = 70; fCutValues[0][3] = 70;
    fCutValues[0][4] = 0.04; fCutValues[0][5] = 0.04; fCutValues[0][6] = 0.05; fCutValues[0][7] = 0.07; fCutValues[0][8] = 2.0;
    fCutValues[0][9] = 1.6;
    fCutValues[0][10] = 1.6;
    fCutValues[0][11] = 1.4;
    fCutValues[0][12] = 0.8; 
    fCutValues[0][13] = 0.97;
    fCutValues[0][14] = 0.97;
    for(int cv=1; cv<kNCutVariations; cv++){
        for(int ct=0; ct<kNCuts; ct++){
            fCutValues[cv][ct] = fCutValues[0][ct];
        }
    }
    // Set Systematic Variations
    fCutValues[1][0] = 80; fCutValues[1][1] = 80; fCutValues[1][2] = 80; fCutValues[1][3] = 80;// 80
    fCutValues[2][4] = 0.104;// 0.104
    fCutValues[3][5] = 0.104;// 0.104
    fCutValues[4][6] = 0.08;// 0.08
    fCutValues[5][7] = 0.1;// 0.1
    fCutValues[6][8] = 1.0;// 1.0
    fCutValues[7][9] = 0.94;// 0.94
    fCutValues[8][10] = 1.41;// 1.41
    fCutValues[9][11] = 4.39;// 4.39
    fCutValues[10][12] = 0.95;// 0.95
    fCutValues[11][13] = 0.99;// 0.99
    fCutValues[12][14] = 0.985;// 0.085
    
    
    
    /*
     TH3F *fXiMinusPiPlus  = new TH3F("fXiMinusPiPlus","Invariant Mass Distribution", 100,0,10, 40,-2,2, 200,1.4,1.6);
     TH3F *fXiMinusPiMinus = new TH3F("fXiMinusPiMinus","Invariant Mass Distribution", 100,0,10, 40,-2,2, 200,1.4,1.6);
     TH3F *fXiPlusPiPlus   = new TH3F("fXiPlusPiPlus","Invariant Mass Distribution", 100,0,10, 40,-2,2, 200,1.4,1.6);
     TH3F *fXiPlusPiMinus  = new TH3F("fXiPlusPiMinus","Invariant Mass Distribution", 100,0,10, 40,-2,2, 200,1.4,1.6);
     */
    
    
    TH1D *hXiMass[8][14]; // = new TH1D("hXiMass","XiMass",200,1.2,1.4);
    TH1D *hXiMassWindow[8][14];
    TH1D *hXiPass[8]; // = new TH1D("hXiPass","XiPass",20,0,20);  // 0: cent 40 - 100
    TH1D *fXiStar[8][14];
    TH1F *fXiM[8][14]; // = new TH1F("fXiM","Invariant Mass Distribution",200,1.4,1.6);
    TH1F *fXiP[8][14]; // = new TH1F("fXiP","Invariant Mass Distribution",200,1.4,1.6);
    TH1F *fXiMPiM[8][14];
    TH1F *fXiPPiP[8][14];
    
    TH1D *fXiStarPt1[8][14];
    TH1D *fXiStarPt2[8][14];
    TH1D *fXiStarPt3[8][14];
    TH1D *fXiStarPt4[8][14];
    TH1D *fXiStarPt5[8][14];
    TH1D *fXiStarPt6[8][14];
    TH1D *fXiStarPt7[8][14];
    TH1D *fXiStarPt8[8][14];
    
       
    for(int ii=0; ii<8; ii++){
        
        hXiPass[ii] = new TH1D(Form("hXiPass_%d",ii),Form("hXiPass_%d",ii),20,-0.5,19.5);
        
        for(int jj=0; jj<14; jj++){
            
            
            hXiMass[ii][jj] = new TH1D(Form("hXiMass_%d_%02d",ii,jj),Form("Xi invariant mass distribution_%d_%02d",ii,jj),200,1.2,1.4);
            hXiMassWindow[ii][jj] = new TH1D(Form("hXiMassWindow_%d_%02d",ii,jj),Form("Xi Window_%d_%02d",ii,jj),200,1.2,1.4);
            fXiStar[ii][jj] = new TH1D(Form("fXiStar_%d_%02d",ii,jj),Form("XiStar invariant mass distribution_%d_%02d",ii,jj),200,1.4,1.6);
            fXiM[ii][jj] = new TH1F(Form("fXiM_%d_%02d",ii,jj),Form("XiStar invariant mass distribution_%d_%02d",ii,jj),200,1.4,1.6);
            fXiP[ii][jj] = new TH1F(Form("fXiP_%d_%02d",ii,jj),Form("XiStar_bar invariant mass distribution_%d_%02d",ii,jj),200,1.4,1.6);
            fXiMPiM[ii][jj] = new TH1F(Form("fXiMPiM_%d_%02d",ii,jj),Form("This is checking for 1860_%d_%02d",ii,jj),600,1.4,2.0);
            fXiPPiP[ii][jj] = new TH1F(Form("fXiPPiP_%d_%02d",ii,jj),Form("BG_%d_%02d",ii,jj),600,1.4,2.0);
            fXiStarPt1[ii][jj] = new TH1D(Form("fXiStarPt1_%d_%02d",ii,jj),Form(" bin1 XiStar invariant mass distribution_%d_%02d",ii,jj),200,1.4,1.6);
            fXiStarPt2[ii][jj] = new TH1D(Form("fXiStarPt2_%d_%02d",ii,jj),Form(" bin2 XiStar invariant mass distribution_%d_%02d",ii,jj),200,1.4,1.6);
            fXiStarPt3[ii][jj] = new TH1D(Form("fXiStarPt3_%d_%02d",ii,jj),Form(" bin3 XiStar invariant mass distribution_%d_%02d",ii,jj),200,1.4,1.6);
            fXiStarPt4[ii][jj] = new TH1D(Form("fXiStarPt4_%d_%02d",ii,jj),Form(" bin4 XiStar invariant mass distribution_%d_%02d",ii,jj),200,1.4,1.6);
            fXiStarPt5[ii][jj] = new TH1D(Form("fXiStarPt5_%d_%02d",ii,jj),Form(" bin5 XiStar invariant mass distribution_%d_%02d",ii,jj),200,1.4,1.6);
            fXiStarPt6[ii][jj] = new TH1D(Form("fXiStarPt6_%d_%02d",ii,jj),Form(" bin6 XiStar invariant mass distribution_%d_%02d",ii,jj),200,1.4,1.6);
            fXiStarPt7[ii][jj] = new TH1D(Form("fXiStarPt7_%d_%02d",ii,jj),Form(" bin7 XiStar invariant mass distribution_%d_%02d",ii,jj),200,1.4,1.6);
            fXiStarPt8[ii][jj] = new TH1D(Form("fXiStarPt8_%d_%02d",ii,jj),Form(" bin8 XiStar invariant mass distribution_%d_%02d",ii,jj),200,1.4,1.6);
            
        }
    }
    
    
    TH1F *evNum = new TH1F("evNum","event numner in file",1000,0,1000);
    
    
  /*
    
    TH1D *SimXi_all = new TH1D("SimXi_all","SimXi_all",200,1.2,1.4);
    TH1D *SimXi_0_10 = new TH1D("SimXi_0_10","SimXi_0_10",200,1.2,1.4);
    TH1D *SimXi_10_20 = new TH1D("SimXi_10_20","SimXi_10_20",200,1.2,1.4);
    TH1D *SimXi_20_30 = new TH1D("SimXi_20_30","SimXi_20_30",200,1.2,1.4);
    TH1D *SimXi_30_40 = new TH1D("SimXi_30_40","SimXi_30_40",200,1.2,1.4);
    TH1D *SimXi_40_50 = new TH1D("SimXi_40_50","SimXi_40_50",200,1.2,1.4);
    TH1D *SimXi_50_60 = new TH1D("SimXi_50_60","SimXi_50_60",200,1.2,1.4);
    TH1D *SimXi_60_70 = new TH1D("SimXi_60_70","SimXi_60_70",200,1.2,1.4);
    TH1D *SimXi_70_100 = new TH1D("SimXi_70_100","SimXi_70_100",200,1.2,1.4);
   */
 /*
    TH1D *SimXibar_all = new TH1D("SimXibar_all","SimXibar_all",200,1.2,1.4);
    TH1D *SimXibar_0_10 = new TH1D("SimXibar_0_10","SimXibar_0_10",200,1.2,1.4);
    TH1D *SimXibar_10_20 = new TH1D("SimXibar_10_20","SimXibar_10_20",200,1.2,1.4);
    TH1D *SimXibar_20_30 = new TH1D("SimXibar_20_30","SimXibar_20_30",200,1.2,1.4);
    TH1D *SimXibar_30_40 = new TH1D("SimXibar_30_40","SimXibar_30_40",200,1.2,1.4);
    TH1D *SimXibar_40_50 = new TH1D("SimXibar_40_50","SimXibar_40_50",200,1.2,1.4);
    TH1D *SimXibar_50_60 = new TH1D("SimXibar_50_60","SimXibar_50_60",200,1.2,1.4);
    TH1D *SimXibar_60_70 = new TH1D("SimXibar_60_70","SimXibar_60_70",200,1.2,1.4);
    TH1D *SimXibar_70_100 = new TH1D("SimXibar_70_100","SimXibar_70_100",200,1.2,1.4);
*/
/*
    TH1D *SimXiStar_all = new TH1D("SimXiStar_all","SimXiStar_all",200,1.4,1.6);
    TH1D *SimXiStar_0_10 = new TH1D("SimXiStar_0_10","SimXiStar_0_10",200,1.4,1.6);
    TH1D *SimXiStar_10_20 = new TH1D("SimXiStar_10_20","SimXiStar_10_20",200,1.4,1.6);
    TH1D *SimXiStar_20_30 = new TH1D("SimXiStar_20_30","SimXiStar_20_30",200,1.4,1.6);
    TH1D *SimXiStar_30_40 = new TH1D("SimXiStar_30_40","SimXiStar_30_40",200,1.4,1.6);
    TH1D *SimXiStar_40_50 = new TH1D("SimXiStar_40_50","SimXiStar_40_50",200,1.4,1.6);
    TH1D *SimXiStar_50_60 = new TH1D("SimXiStar_50_60","SimXiStar_50_60",200,1.4,1.6);
    TH1D *SimXiStar_60_70 = new TH1D("SimXiStar_60_70","SimXiStar_60_70",200,1.4,1.6);
    TH1D *SimXiStar_70_100 = new TH1D("SimXiStar_70_100","SimXiStar_70_100",200,1.4,1.6);
 */
/*
    TH1D *SimXiStarbar_all = new TH1D("SimXiStarbar_all","SimXiStarbar_all",200,1.2,1.4);
    TH1D *SimXiStarbar_0_10 = new TH1D("SimXiStarbar_0_10","SimXiStarbar_0_10",200,1.2,1.4);
    TH1D *SimXiStarbar_10_20 = new TH1D("SimXiStarbar_10_20","SimXiStarbar_10_20",200,1.2,1.4);
    TH1D *SimXiStarbar_20_30 = new TH1D("SimXiStarbar_20_30","SimXiStarbar_20_30",200,1.2,1.4);
    TH1D *SimXiStarbar_30_40 = new TH1D("SimXiStarbar_30_40","SimXiStarbar_30_40",200,1.2,1.4);
    TH1D *SimXiStarbar_40_50 = new TH1D("SimXiStarbar_40_50","SimXiStarbar_40_50",200,1.2,1.4);
    TH1D *SimXiStarbar_50_60 = new TH1D("SimXiStarbar_50_60","SimXiStarbar_50_60",200,1.2,1.4);
    TH1D *SimXiStarbar_60_70 = new TH1D("SimXiStarbar_60_70","SimXiStarbar_60_70",200,1.2,1.4);
    TH1D *SimXiStarbar_70_100 = new TH1D("SimXiStarbar_70_100","SimXiStarbar_70_100",200,1.2,1.4);
 */
    
    
    TH1F *hCheckEN = new TH1F("hCheckEN","Checking Event Number",3,0,3);
    
    
    Double_t px1,py1,pz1, px2,py2,pz2;
    Double_t p1sq,p2sq,e1,e2,angle;
    Double_t  fTrueMassPr=.93827, fTrueMassPi=.13957, fTrueMassK=.493677, fTrueMassLam=1.11568, fTrueMassXi=1.32171, fMassWindow=0.007;
    Double_t xiMass, xiStarMass;
    Double_t xiP[3], xiStarP[3];
    Double_t xiStarMom;
    
    Double_t xiStarPt, xiStarY;
    
    
    TTree *fTreeEvent = (TTree*)f->Get("fTreeEvent");
    fTreeEvent->SetBranchAddress("fTreeVariablePrimaryVz",&fTreeVariablePrimaryVz);
    fTreeEvent->SetBranchAddress("fTreeVariableMult",&fTreeVariableMult);
   // fTreeEvent->SetBranchAddress("fTreeVariableEventCounter1",&fTreeVariableEventCounter1);
    fTreeEvent->SetBranchAddress("fTreeVariableCentrality",&fTreeVariableCentrality);
    fTreeEvent->SetBranchAddress("fTreeVariableCentralityV0M",&fTreeVariableCentralityV0M);
    fTreeEvent->SetBranchAddress("fTreeVariableCentralityTRK",&fTreeVariableCentralityTRK);
    fTreeEvent->SetBranchAddress("fTreeVariableCentralitySPD",&fTreeVariableCentralitySPD);
    fTreeEvent->SetBranchAddress("fTreeVariableRunNumber",&fTreeVariableRunNumber);
    fTreeEvent->SetBranchAddress("fTreeVariableEventNumber1",&fTreeVariableEventNumber1);
    fTreeEvent->SetBranchAddress("fTreeVariableEventNumberInFile",&fTreeVariableEventNumberInFile);
    fTreeEvent->SetBranchAddress("fTreeVariablemyTracks",&fTreeVariablemyTracks);
    fTreeEvent->SetBranchAddress("fTreeVariablexiTracks",&fTreeVariablexiTracks);
    
    
    TTree *fTreeXi = (TTree*)f->Get("fTreeXi");
    fTreeXi->SetBranchAddress("fTreeVariableXiPt",&fTreeVariableXiPt);
    fTreeXi->SetBranchAddress("fTreeVariableEta",&fTreeVariableEta);
    fTreeXi->SetBranchAddress("fTreeVariablePhi",&fTreeVariablePhi);
    fTreeXi->SetBranchAddress("fTreeVariableXiMass",&fTreeVariableXiMass);
    fTreeXi->SetBranchAddress("fTreeVariableDecayXYxi",&fTreeVariableDecayXYxi);
    fTreeXi->SetBranchAddress("fTreeVariableDecayXYlambda",&fTreeVariableDecayXYlambda);
    fTreeXi->SetBranchAddress("fTreeVariableNclusp",&fTreeVariableNclusp);
    fTreeXi->SetBranchAddress("fTreeVariableNcluspi1",&fTreeVariableNcluspi1);
    fTreeXi->SetBranchAddress("fTreeVariableNcluspi2",&fTreeVariableNcluspi2);
    fTreeXi->SetBranchAddress("fTreeVariablePVdcaP",&fTreeVariablePVdcaP);
    fTreeXi->SetBranchAddress("fTreeVariablePVdcaPi1",&fTreeVariablePVdcaPi1);
    fTreeXi->SetBranchAddress("fTreeVariablePVdcaPi2",&fTreeVariablePVdcaPi2);
    fTreeXi->SetBranchAddress("fTreeVariablePVdcaLambda",&fTreeVariablePVdcaLambda);
    fTreeXi->SetBranchAddress("fTreeVariableDCApip",&fTreeVariableDCApip);
    fTreeXi->SetBranchAddress("fTreeVariableDCApilambda",&fTreeVariableDCApilambda);
    fTreeXi->SetBranchAddress("fTreevariableCosPAlambda",&fTreevariableCosPAlambda);
    fTreeXi->SetBranchAddress("fTreevariableCosPAxi",&fTreevariableCosPAxi);
    fTreeXi->SetBranchAddress("fTreeVariableXiTrackX",&fTreeVariableXiTrackX);
    fTreeXi->SetBranchAddress("fTreeVariableXiTrackY",&fTreeVariableXiTrackY);
    fTreeXi->SetBranchAddress("fTreeVariableXiTrackZ",&fTreeVariableXiTrackZ);
    fTreeXi->SetBranchAddress("fTreeVariableXiTrackPx",&fTreeVariableXiTrackPx);
    fTreeXi->SetBranchAddress("fTreeVariableXiTrackPy",&fTreeVariableXiTrackPy);
    fTreeXi->SetBranchAddress("fTreeVariableXiTrackPz",&fTreeVariableXiTrackPz);
    fTreeXi->SetBranchAddress("fTreeVariableXiTrackQ",&fTreeVariableXiTrackQ);
    fTreeXi->SetBranchAddress("fTreeVariableXiTrackpID",&fTreeVariableXiTrackpID);
    fTreeXi->SetBranchAddress("fTreeVariableXiTracknID",&fTreeVariableXiTracknID);
    fTreeXi->SetBranchAddress("fTreeVariableXiTrackbID",&fTreeVariableXiTrackbID);
   // fTreeXi->SetBranchAddress("fTreeVariableEventCounter2",&fTreeVariableEventCounter2);
    fTreeXi->SetBranchAddress("fTreeVariableEventNumber2",&fTreeVariableEventNumber2);
    
    
    TTree *fTreePi = (TTree*)f->Get("fTreePi");
    fTreePi->SetBranchAddress("fTreeVariableESDTrackX",&fTreeVariableESDTrackX);
    fTreePi->SetBranchAddress("fTreeVariableESDTrackY",&fTreeVariableESDTrackY);
    fTreePi->SetBranchAddress("fTreeVariableESDTrackZ",&fTreeVariableESDTrackZ);
    fTreePi->SetBranchAddress("fTreeVariableESDTrackPx",&fTreeVariableESDTrackPx);
    fTreePi->SetBranchAddress("fTreeVariableESDTrackPy",&fTreeVariableESDTrackPy);
    fTreePi->SetBranchAddress("fTreeVariableESDTrackPz",&fTreeVariableESDTrackPz);
    fTreePi->SetBranchAddress("fTreeVariableESDTrackQ",&fTreeVariableESDTrackQ);
    fTreePi->SetBranchAddress("fTreeVariableESDTrackID",&fTreeVariableESDTrackID);
    fTreePi->SetBranchAddress("fTreeVariableESDtrackDCAZ",&fTreeVariableESDtrackDCAZ);
    fTreePi->SetBranchAddress("fTreeVariableESDtrackDCAXY",&fTreeVariableESDtrackDCAXY);
    fTreePi->SetBranchAddress("fTreeVariableESDtrackEta",&fTreeVariableESDtrackEta);
    fTreePi->SetBranchAddress("fTreeVariableESDtrackPt",&fTreeVariableESDtrackPt);
    fTreePi->SetBranchAddress("fTreeVariableESDtrackMom",&fTreeVariableESDtrackMom);
    fTreePi->SetBranchAddress("fTreeVariableESDtrackfNclusTPC",&fTreeVariableESDtrackfNclusTPC);
 //   fTreePi->SetBranchAddress("fTreeVariableEventCounter3",&fTreeVariableEventCounter3);
    fTreePi->SetBranchAddress("fTreeVariavleESDStatus",&fTreeVariavleESDStatus);
    fTreePi->SetBranchAddress("fTreeVariableEventNumber3",&fTreeVariableEventNumber3);
    
    
    TTree *fTreeMC = (TTree*)f->Get("fTreeMC");
    fTreeMC->SetBranchAddress("fTreeVariableMCinputTotalXiPt",&fTreeVariableMCinputTotalXiPt);
    fTreeMC->SetBranchAddress("fTreeVariableMCinputTotalXiY",&fTreeVariableMCinputTotalXiY);
    fTreeMC->SetBranchAddress("fTreeVariableMCinputTotalXiCalcMass",&fTreeVariableMCinputTotalXiCalcMass);
    fTreeMC->SetBranchAddress("fTreeVariableMCinputTotalXibarPt",&fTreeVariableMCinputTotalXibarPt);
    fTreeMC->SetBranchAddress("fTreeVariableMCinputTotalXibarY",&fTreeVariableMCinputTotalXibarY);
    fTreeMC->SetBranchAddress("fTreeVariableMCinputTotalXibarCalcMass",&fTreeVariableMCinputTotalXibarCalcMass);
    
    
    fTreeMC->SetBranchAddress("fTreeVariableMCinputTotalXiStarPt",&fTreeVariableMCinputTotalXiStarPt);
    fTreeMC->SetBranchAddress("fTreeVariableMCinputTotalXiStarY",&fTreeVariableMCinputTotalXiStarY);
    fTreeMC->SetBranchAddress("fTreeVariableMCinputTotalXiStarCalcMass",&fTreeVariableMCinputTotalXiStarCalcMass);
    fTreeMC->SetBranchAddress("fTreeVariableMCinputTotalXiStarbarPt",&fTreeVariableMCinputTotalXiStarbarPt);
    fTreeMC->SetBranchAddress("fTreeVariableMCinputTotalXiStarbarY",&fTreeVariableMCinputTotalXiStarbarY);
    fTreeMC->SetBranchAddress("fTreeVariableMCinputTotalXiStarbarCalcMass",&fTreeVariableMCinputTotalXiStarbarCalcMass);
    fTreeMC->SetBranchAddress("fTreeVariableMCinputTotalSongPID",&fTreeVariableMCinputTotalSongPID);
    fTreeMC->SetBranchAddress("fTreeVariableMCinputTotalCent",&fTreeVariableMCinputTotalCent);
    
    
    for (Long64_t ev=0; ev<fTreeEvent->GetEntries(); ev++){
        //cout << "fTreeEvent loop : " << ev ;
        
        fTreeEvent->GetEntry(ev);
        evNum->Fill(fTreeVariableEventNumberInFile);
        //cout << " is loaded" << endl;
        
        totxiTracks = totxiTracks + fTreeVariablexiTracks;
        totmyTracks = totmyTracks + fTreeVariablemyTracks;
        
        
        //if(fTreeVariablexiTracks==0) cout << Form("Xi Tracks : +%04d     Sum : %06d",fTreeVariablexiTracks,totxiTracks) << endl;
        //cout << Form("My Tracks : +%04d     Sum : %06d",fTreeVariablemyTracks,totmyTracks) << endl;
        
        if(fTreeVariablexiTracks==0 || fTreeVariablemyTracks==0) continue;
        if(fTreeVariablePrimaryVz<-10.||fTreeVariablePrimaryVz>10.) continue;
        
        
        
        if(fTreeVariableCentrality<0.||fTreeVariableCentrality >=100.) continue;
        if(fTreeVariableCentrality<10.)         ct=0;
        else if(fTreeVariableCentrality<20.)         ct=1;
        else if(fTreeVariableCentrality<30.)         ct=2;
        else if(fTreeVariableCentrality<40.)         ct=3;
        else if(fTreeVariableCentrality<50.)         ct=4;
        else if(fTreeVariableCentrality<60.)         ct=5;
        else if(fTreeVariableCentrality<70.)         ct=6;
        else ct=7;
        
        
        
        
        
        
        for (Long64_t i=0; i<fTreeVariablexiTracks;i++){
            
            fTreeXi->GetEntry(totxiTracks-fTreeVariablexiTracks+i);
            
            //if(fTreeVariableEventCounter2 != fTreeVariableEventCounter1) cout << fTreeVariableEventCounter2 << "  " << fTreeVariableEventCounter1 << endl;
            //if(fTreeVariableEventCounter2!= fTreeVariableEventCounter1) continue;  // event counter method
            
            
            if(fTreeVariableEventNumber2 != fTreeVariableEventNumber1) hCheckEN->Fill(1);
            if(fTreeVariableEventNumber2 != fTreeVariableEventNumber1) continue; // event number method
            
            
            if(fTreeVariableXiPt<.15||fTreeVariableXiPt>1000) continue;
            
            nc = 0;
            
            // hXiPass->Fill(nc);
            /* if(fTreeVariableNclusp<fCutValues[0][0]) ++nc;  //continue; hXiPass->Fill(++nc);
             if(fTreeVariableNcluspi1<fCutValues[0][1]) ++nc; //continue; hXiPass->Fill(++nc);
             if(fTreeVariableNcluspi2<fCutValues[0][2]) ++nc; //continue; hXiPass->Fill(++nc);
             if(fTreeVariablePVdcaP<fCutValues[0][4]) ++nc; //continue; hXiPass->Fill(++nc);
             if(fTreeVariablePVdcaPi1<fCutValues[0][5]) ++nc; //continue; hXiPass->Fill(++nc);
             if(fTreeVariablePVdcaPi2<fCutValues[0][6]) ++nc; //continue; hXiPass->Fill(++nc);
             if(fTreeVariablePVdcaLambda<fCutValues[0][7]) ++nc; //continue; hXiPass->Fill(++nc);
             if(fTreeVariableDCApip>fCutValues[0][9]) ++nc; //continue; hXiPass->Fill(++nc);
             if(fTreeVariableDCApilambda>fCutValues[0][10]) ++nc; //continue; hXiPass->Fill(++nc);
             if(fTreeVariableDecayXYlambda<fCutValues[0][11]) ++nc; //continue; hXiPass->Fill(++nc);
             if(fTreeVariableDecayXYxi<fCutValues[0][12]) ++nc; //continue; hXiPass->Fill(++nc);
             if(fTreevariableCosPAlambda<fCutValues[0][13])++nc; // continue; hXiPass->Fill(++nc);
             if(fTreevariableCosPAxi<fCutValues[0][14]) ++nc; //continue; hXiPass->Fill(++nc);
             */
            
            
            while(1){
                if(fTreeVariableNclusp<fCutValues[0][0]) break;
                else nc=1;
                if(fTreeVariableNcluspi1<fCutValues[0][1]) break;
                else nc=2;
                if(fTreeVariableNcluspi2<fCutValues[0][2]) break;
                else nc=3;
                if(fTreeVariablePVdcaP<fCutValues[0][4]) break;
                else nc=4;
                if(fTreeVariablePVdcaPi1<fCutValues[0][5])break;
                else nc=5;
                if(fTreeVariablePVdcaPi2<fCutValues[0][6]) break;
                else nc=6;
                if(fTreeVariablePVdcaLambda<fCutValues[0][7]) break;
                else nc=7;
                if(fTreeVariableDCApip>fCutValues[0][9])break;
                else nc=8;
                if(fTreeVariableDCApilambda>fCutValues[0][10])break;
                else nc=9;
                if(fTreeVariableDecayXYlambda<fCutValues[0][11])break;
                else nc=10;
                if(fTreeVariableDecayXYxi<fCutValues[0][12]) break;
                else nc=11;
                if(fTreevariableCosPAlambda<fCutValues[0][13]) break;
                else nc=12;
                if(fTreevariableCosPAxi<fCutValues[0][14]) break;
                else nc=13;
                break; // if Xi candidate passes all cut, exit the loop
            }
            
            //     for(int ii=0; ii<4; ii++){
            //         if(ii>ct) continue;
            for(int jj=0; jj<14; jj++) {
                if(jj>nc) continue;
                hXiPass[ct]->Fill(jj);
            }
            //     }
            
            
            
            //      if(fTreeVariableDecayXYxi>100) continue; hXiPass->Fill(++nc);
            
            
            //      if(fTreeVariableXiPt>10) continue; hXiPass->Fill(++nc);
            //      if(fTreeVariableEta<-2 || fTreeVariableEta>2) continue; hXiPass->Fill(++nc);
            
            //      if(fTreeVariableXiMass>1.4) continue; hXiPass->Fill(++nc);
            
            
            
            
            //     for(int ii=0; ii<4; ii++){
            //        if(ii>ct) continue;
            for(int jj=0; jj<14; jj++){
                if(jj>nc) continue;
                hXiMass[ct][jj]->Fill(fTreeVariableXiMass);
                
            }
            
            
            //    }
            
            
            if(fabs(fTreeVariableXiMass-fTrueMassXi)>fMassWindow) continue;
            
            
            //     for(int ii=0; ii<4; ii++){
            //         if(ii>ct) continue;
            for(int jj=0; jj<14; jj++){
                if(jj>nc) continue;
                hXiMassWindow[ct][jj]->Fill(fTreeVariableXiMass);
                
            }
            
            
            //      }
            
            
            
            //////////////////////////////////////////////////////////
            // Reconstruct Xi(1530)
            //    for(Int_t EN=0; EN<fEventsToMix+1; EN++){// Event buffer loop
            //mypions = 0;
            for(Long64_t l=0; l<fTreeVariablemyTracks; l++){// Present(EN=0) and Past(EN from 1 to fEventsToMix) event track loop
                
                fTreePi->GetEntry(totmyTracks-fTreeVariablemyTracks+l);
                // if(fTreeVariableEventCounter3 != fTreeVariableEventCounter1) cout << "Different!!" << endl;
                // if(fTreeVariableEventCounter3 != fTreeVariableEventCounter1) continue;
                
                if(fTreeVariableEventNumber3 != fTreeVariableEventNumber1) hCheckEN->Fill(2);
                if(fTreeVariableEventNumber3 != fTreeVariableEventNumber1) continue; // evnet number method
                
                
                
                if(fTreeVariableESDtrackPt<.15||fTreeVariableESDtrackPt>1000) continue;
                
                if(EN==0) {
                    //    if((fEvt+EN)->fTracks[l].fID == pTrackXi->GetID()) continue;
                    //    if((fEvt+EN)->fTracks[l].fID == nTrackXi->GetID()) continue;
                    //    if((fEvt+EN)->fTracks[l].fID == bTrackXi->GetID()) continue;
                    
                    if(fTreeVariableESDTrackID==fTreeVariableXiTrackpID) continue;
                    if(fTreeVariableESDTrackID==fTreeVariableXiTracknID) continue;
                    if(fTreeVariableESDTrackID==fTreeVariableXiTrackbID) continue;
                    
                    
                } // if(EN==0)
                
                
                
                
                //     fXiTrack->Set(xiVtx, xiP, fCovMatrix, Short_t(xiCharge));
                
                
                
                //        if(!fESDTrack4) continue;
                //       fESDTrack4->Set((fEvt+EN)->fTracks[l].fX, (fEvt+EN)->fTracks[l].fP, (fEvt+EN)->fTracks[l].fCov, (fEvt+EN)->fTracks[l].fCharge);
                /*      if(fAODcase){
                 if((Bool_t)(((1<<5) & (fEvt+EN)->fTracks[l].fFilterMap) == 0)) continue;// AOD filterbit cut, "Standard cuts with tight dca"
                 }else{  */
                
                //       fDecayParameters[8] = (fEvt+EN)->fTracks[l].fDCAXY;// DCA Vtx pion third
                //       ((TH1F*)fOutputList->FindObject("fDCADist_3rd_pi"))->Fill(fDecayParameters[8]);
                
                
                
                if(fTreeVariableESDtrackDCAZ > 2.) continue; //3rd pion DCAz cut
                
                //if( ((fTreeVariavleESDStatus)&AliESDtrack::kITSrefit)==0) continue;// Require itsrefit 20140317
                // no Chi^2 cut applied for ESDs.  Info not available in my track structure.
                //  } // if(fAODcase)
                
                
                if(fabs(fTreeVariableESDtrackEta) > 0.8) continue;
                
                //     fDecayParameters[3] = (fEvt+EN)->fTracks[l].fNclusTPC;
                //     ((TH1F*)fOutputList->FindObject("fTPCNcls_pi3"))->Fill(fDecayParameters[3]);
                
                
                
                // added    AliVertex *XiStarVtx = new AliVertex((fEvt+EN)->fTracks[l].fX,0,0);
                //fESDTrack4->PropagateToDCA(fXiTrack, bField);// Propagate tracks to dca, both tracks are budged
                // added    if(!(fXiTrack->PropagateToDCA(XiStarVtx, bField, 3))) continue;// Propagate tracks to dca, version which assumes fESDTrack4 is already primary
                
                
                /*
                 
                 /////////////
                 fXiTrack->GetPxPyPz(pDaughter1);
                 fXiTrack->GetXYZ(xDaughter1);
                 fESDTrack4->GetPxPyPz(pDaughter2);
                 fESDTrack4->GetXYZ(xDaughter2);
                 //////////////////////////
                 
                 */
                
                //xiStarVtx[0] = (xDaughter1[0]+xDaughter2[0])/2.;
                //xiStarVtx[1] = (xDaughter1[1]+xDaughter2[1])/2.;
                //xiStarVtx[2] = (xDaughter1[2]+xDaughter2[2])/2.;
                //decayLength = sqrt(pow(xiStarVtx[0]-primaryVtx[0],2)+pow(xiStarVtx[1]-primaryVtx[1],2)+pow(xiStarVtx[2]-primaryVtx[2],2));
                
                px1=fTreeVariableXiTrackPx;
                py1=fTreeVariableXiTrackPy;
                pz1=fTreeVariableXiTrackPz;
                px2=fTreeVariableESDTrackPx;
                py2=fTreeVariableESDTrackPy;
                pz2=fTreeVariableESDTrackPz;
                
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
                //xiStarE = e1 + e2;
                
                
                //if( (xiStarP[0]*(xiStarVtx[0]-primaryVtx[0]) + xiStarP[1]*(xiStarVtx[1]-primaryVtx[1]) + xiStarP[2]*(xiStarVtx[2]-primaryVtx[2]))/xiStarMom/decayLength < fXiStarCosTheta) continue;
                
                
                
                for(int cv=0; cv<kNCutVariations; cv++){
                    
                    //        if(fTreeVariableNclusp < fCutValues[cv][0]) continue;// Nclus proton
                    //        if(fTreeVariableNcluspi1 < fCutValues[cv][1]) continue;// Nclus pion first
                    //        if(fTreeVariableNcluspi2 < fCutValues[cv][2]) continue;// Nclus pion second
                    if(fTreeVariableESDtrackfNclusTPC < fCutValues[cv][3]) continue;// Nclus pion third
                    
                    //
                    
                    //       if(fTreeVariablePVdcaP < fCutValues[cv][4]) continue;// DCAVtx proton
                    //       if(fTreeVariablePVdcaPi1 < fCutValues[cv][5]) continue;// DCAVtx pion first
                    //       if(fTreeVariablePVdcaPi2 < fCutValues[cv][6]) continue;// DCAVtx pion second
                    //        if(fTreeVariablePVdcaLambda < fCutValues[cv][7]) continue;// DCAVtx Lambda
                    
                    if(cv!=8) {if(fTreeVariableESDtrackDCAXY > (0.0182 + 0.035/pow(fTreeVariableESDtrackPt,1.01))) continue;}// DCAVtx pion third
                    else {if(fTreeVariableESDtrackDCAXY > fCutValues[cv][8]) continue;}// DCAVtx pion third
                    //
                    //         if(fTreeVariableDCApip > fCutValues[cv][9]) continue;// DCAV proton-pion
                    //         if(fTreeVariableDCApilambda > fCutValues[cv][10]) continue;// DCAV Lambda-pion
                    //
                    //          if(fTreeVariableDecayXYlambda < fCutValues[cv][11]) continue;// Rxy Lambda
                    //          if(fTreeVariableDecayXYxi < fCutValues[cv][12]) continue;// Rxy Xi
                    //
                    //          if(fTreevariableCosPAlambda < fCutValues[cv][13]) continue;// Cos PA Lambda
                    //          if(fTreevariableCosPAxi < fCutValues[cv][14]) continue;// Cos PA Xi
                    
                    
                    
                    
                    
                    
                    if(EN==0){
                        if (!(cv == 0)) continue;
                        
                        if(fTreeVariableXiTrackQ == -1 && fTreeVariableESDTrackQ == -1){ //fXiMPiM->Fill(xiStarMass); // fXiMinusPiMinus->Fill(xiStarPt, xiStarY, xiStarMass);
                            //   for(int ii=0; ii<4; ii++){
                            //        if(ii>ct) continue;
                            for(int jj=0; jj<14; jj++){
                                if(jj>nc) continue;
                                fXiMPiM[ct][jj]->Fill(xiStarMass);
                            }
                            //     }
                        }
                        
                        else if(fTreeVariableXiTrackQ == -1 && fTreeVariableESDTrackQ == +1) {//fXiM->Fill(xiStarMass); //fXiMinusPiPlus->Fill(xiStarPt, xiStarY, xiStarMass);
                            //fXiM->Fill(xiStarMass);
                            //       for(int ii=0; ii<4; ii++){
                            //             if(ii>ct) continue;
                            for(int jj=0; jj<14; jj++){
                                if(jj>nc) continue;
                                fXiM[ct][jj]->Fill(xiStarMass);
                                fXiStar[ct][jj]->Fill(xiStarMass);
                                
                                if(fTreeVariableXiPt>0.8 && fTreeVariableXiPt<1.2)fXiStarPt1[ct][jj]->Fill(xiStarMass);
                                if(fTreeVariableXiPt>1.2 && fTreeVariableXiPt<1.6)fXiStarPt2[ct][jj]->Fill(xiStarMass);
                                if(fTreeVariableXiPt>1.6 && fTreeVariableXiPt<2.0)fXiStarPt3[ct][jj]->Fill(xiStarMass);
                                if(fTreeVariableXiPt>2.0 && fTreeVariableXiPt<2.4)fXiStarPt4[ct][jj]->Fill(xiStarMass);
                                if(fTreeVariableXiPt>2.4 && fTreeVariableXiPt<3.2)fXiStarPt5[ct][jj]->Fill(xiStarMass);
                                if(fTreeVariableXiPt>3.2 && fTreeVariableXiPt<4.0)fXiStarPt6[ct][jj]->Fill(xiStarMass);
                                if(fTreeVariableXiPt>4.0 && fTreeVariableXiPt<4.8)fXiStarPt7[ct][jj]->Fill(xiStarMass);
                                if(fTreeVariableXiPt>4.8 && fTreeVariableXiPt<5.6)fXiStarPt8[ct][jj]->Fill(xiStarMass);

                            }
                            //        }
                        }
                        else if(fTreeVariableXiTrackQ == +1 && fTreeVariableESDTrackQ == -1){ // fXiP->Fill(xiStarMass); // fXiPlusPiMinus->Fill(xiStarPt, xiStarY, xiStarMass);
                            //       for(int ii=0; ii<4; ii++){
                            //             if(ii>ct) continue;
                            for(int jj=0; jj<14; jj++){
                                if(jj>nc) continue;
                                fXiP[ct][jj]->Fill(xiStarMass);
                                fXiStar[ct][jj]->Fill(xiStarMass);
                                if(fTreeVariableXiPt>0.8 && fTreeVariableXiPt<1.2)fXiStarPt1[ct][jj]->Fill(xiStarMass);
                                if(fTreeVariableXiPt>1.2 && fTreeVariableXiPt<1.6)fXiStarPt2[ct][jj]->Fill(xiStarMass);
                                if(fTreeVariableXiPt>1.6 && fTreeVariableXiPt<2.0)fXiStarPt3[ct][jj]->Fill(xiStarMass);
                                if(fTreeVariableXiPt>2.0 && fTreeVariableXiPt<2.4)fXiStarPt4[ct][jj]->Fill(xiStarMass);
                                if(fTreeVariableXiPt>2.4 && fTreeVariableXiPt<3.2)fXiStarPt5[ct][jj]->Fill(xiStarMass);
                                if(fTreeVariableXiPt>3.2 && fTreeVariableXiPt<4.0)fXiStarPt6[ct][jj]->Fill(xiStarMass);
                                if(fTreeVariableXiPt>4.0 && fTreeVariableXiPt<4.8)fXiStarPt7[ct][jj]->Fill(xiStarMass);
                                if(fTreeVariableXiPt>4.8 && fTreeVariableXiPt<5.6)fXiStarPt8[ct][jj]->Fill(xiStarMass);

                            }
                            //          }
                        }
                        
                        //fXiP->Fill(xiStarMass);
                        else {//fXiPPiP->Fill(xiStarMass);//fXiPlusPiPlus->Fill(xiStarPt, xiStarY, xiStarMass)
                            //           for(int ii=0; ii<4; ii++){
                            //               if(ii>ct) continue;
                            for(int jj=0; jj<14; jj++){
                                if(jj>nc) continue;
                                fXiPPiP[ct][jj]->Fill(xiStarMass);

                            }
                            //           }
                            
                        }
                        
                        
                    }/*else {
                      if(fXiTrack->Charge() == -1 &&  fESDTrack4->Charge() == -1) CutVar[cv].fXiMinusPiMinusbkg->Fill(xiStarPt, xiStarY, xiStarMass);
                      else if(fXiTrack->Charge() == -1 &&  fESDTrack4->Charge() == +1) CutVar[cv].fXiMinusPiPlusbkg->Fill(xiStarPt, xiStarY, xiStarMass);
                      else if(fXiTrack->Charge() == +1 &&  fESDTrack4->Charge() == -1) CutVar[cv].fXiPlusPiMinusbkg->Fill(xiStarPt, xiStarY, xiStarMass);
                      else CutVar[cv].fXiPlusPiPlusbkg->Fill(xiStarPt, xiStarY, xiStarMass);
                      }
                      */
                    
                    /*
                     // MC associaton AOD
                     if(fMCcase && mcXiFilled && EN==0 && fAODcase){// AOD MC's
                     
                     MCXiStarD2 = (AliAODMCParticle*)mcArray->At(abs((fEvt)->fTracks[l].fLabel));
                     
                     if(abs(MCXiStarD2->GetPdgCode())==kPionCode){
                     if(MCXi->GetMother() == MCXiStarD2->GetMother()){
                     MCXiStar = (AliAODMCParticle*)mcArray->At(MCXi->GetMother());
                     if(abs(MCXiStar->GetPdgCode())==kXiStarCode) {
                     
                     if(fXiTrack->Charge() == -1 &&  fESDTrack4->Charge() == +1) fMCrecXiMinusPiPlus->Fill(xiStarPt, xiStarY, xiStarMass);
                     if(fXiTrack->Charge() == +1 &&  fESDTrack4->Charge() == -1) fMCrecXiPlusPiMinus->Fill(xiStarPt, xiStarY, xiStarMass);
                     
                     }
                     }
                     }
                     }
                     */
                    
                    // MC associaton ESD
                    /*
                     if(fMCcase && mcXiFilled && EN==0 && !fAODcase){// ESD MC's
                     MCXiStarD2esd = (TParticle*)mcstack->Particle(abs((fEvt)->fTracks[l].fLabel));
                     
                     if(abs(MCXiStarD2esd->GetPdgCode())==kPionCode){
                     if(MCXiesd->GetMother(0) == MCXiStarD2esd->GetMother(0)){
                     
                     MCXiStaresd = (TParticle*)mcstack->Particle(abs(MCXiesd->GetMother(0)));
                     if(abs(MCXiStaresd->GetPdgCode())==kXiStarCode) {
                     
                     if(fXiTrack->Charge() == -1 &&  fESDTrack4->Charge() == +1) CutVar[cv].fMCrecXiMinusPiPlus->Fill(xiStarPt, xiStarY, xiStarMass);
                     if(fXiTrack->Charge() == +1 &&  fESDTrack4->Charge() == -1) CutVar[cv].fMCrecXiPlusPiMinus->Fill(xiStarPt, xiStarY, xiStarMass);
                     
                     }
                     }
                     }
                     }
                     */
             /*
                    for(int jj=0; jj<14; jj++){
                        if(jj>nc) continue;
                        fXiStar[ct][jj]->Add(fXiP[ct][jj]);
                        fXiStar[ct][jj]->Add(fXiM[ct][jj]);
                    }
              */

                }// Cut Variation loop
                
                //                mypions++;
                
            }// 3rd pion loop
            
            
            //      }// Event mixing loop
            
            
            
            //            cout << "mypions : " << mypions << endl;
            
            
            
            
            
            
        } // Xi loop
        
        
        if (fTreeEvent->GetEntries()%30 == 0) cout << "Processing : " << ev << "/" << fTreeEvent->GetEntries() << endl;
        
    } // Base loop
    
/*
    for(Long64_t imc=0; imc<fTreeMC->GetEntries(); imc++) {
        
        fTreeMC->GetEntry(imc);
        
        SimXi_all->Fill(fTreeVariableMCinputTotalXiCalcMass);
        SimXi_all->Fill(fTreeVariableMCinputTotalXibarCalcMass);
        
        SimXiStar_all->Fill(fTreeVariableMCinputTotalXiStarCalcMass);
        SimXiStar_all->Fill(fTreeVariableMCinputTotalXiStarbarCalcMass);
        
        
        if(fTreeVariableMCinputTotalCent<0.||fTreeVariableMCinputTotalCent >=100.) continue;
    
        
        if(fTreeVariableMCinputTotalCent<10.){
            SimXi_0_10->Fill(fTreeVariableMCinputTotalXiCalcMass);
            SimXi_0_10->Fill(fTreeVariableMCinputTotalXibarCalcMass);
            SimXiStar_0_10->Fill(fTreeVariableMCinputTotalXiStarCalcMass);
            SimXiStar_0_10->Fill(fTreeVariableMCinputTotalXiStarbarCalcMass);

        }
        else if(fTreeVariableMCinputTotalCent<20.){
            SimXi_10_20->Fill(fTreeVariableMCinputTotalXiCalcMass);
            SimXi_10_20->Fill(fTreeVariableMCinputTotalXibarCalcMass);
            SimXiStar_10_20->Fill(fTreeVariableMCinputTotalXiStarCalcMass);
            SimXiStar_10_20->Fill(fTreeVariableMCinputTotalXiStarbarCalcMass);
            
        }
        else if(fTreeVariableMCinputTotalCent<30.){
            SimXi_20_30->Fill(fTreeVariableMCinputTotalXiCalcMass);
            SimXi_20_30->Fill(fTreeVariableMCinputTotalXibarCalcMass);
            SimXiStar_20_30->Fill(fTreeVariableMCinputTotalXiStarCalcMass);
            SimXiStar_20_30->Fill(fTreeVariableMCinputTotalXiStarbarCalcMass);
            
        }
        else if(fTreeVariableMCinputTotalCent<40.){
            SimXi_30_40->Fill(fTreeVariableMCinputTotalXiCalcMass);
            SimXi_30_40->Fill(fTreeVariableMCinputTotalXibarCalcMass);
            SimXiStar_30_40->Fill(fTreeVariableMCinputTotalXiStarCalcMass);
            SimXiStar_30_40->Fill(fTreeVariableMCinputTotalXiStarbarCalcMass);
            
        }
        else if(fTreeVariableMCinputTotalCent<50.){
            SimXi_40_50->Fill(fTreeVariableMCinputTotalXiCalcMass);
            SimXi_40_50->Fill(fTreeVariableMCinputTotalXibarCalcMass);
            SimXiStar_40_50->Fill(fTreeVariableMCinputTotalXiStarCalcMass);
            SimXiStar_40_50->Fill(fTreeVariableMCinputTotalXiStarbarCalcMass);
            
        }
        else if(fTreeVariableMCinputTotalCent<60.){
            SimXi_50_60->Fill(fTreeVariableMCinputTotalXiCalcMass);
            SimXi_50_60->Fill(fTreeVariableMCinputTotalXibarCalcMass);
            SimXiStar_50_60->Fill(fTreeVariableMCinputTotalXiStarCalcMass);
            SimXiStar_50_60->Fill(fTreeVariableMCinputTotalXiStarbarCalcMass);
            
        }
        else if(fTreeVariableMCinputTotalCent<70.){
            SimXi_60_70->Fill(fTreeVariableMCinputTotalXiCalcMass);
            SimXi_60_70->Fill(fTreeVariableMCinputTotalXibarCalcMass);
            SimXiStar_60_70->Fill(fTreeVariableMCinputTotalXiStarCalcMass);
            SimXiStar_60_70->Fill(fTreeVariableMCinputTotalXiStarbarCalcMass);
            
        }
        
        else {
            SimXi_70_100->Fill(fTreeVariableMCinputTotalXiCalcMass);
            SimXi_70_100->Fill(fTreeVariableMCinputTotalXibarCalcMass);
            SimXiStar_70_100->Fill(fTreeVariableMCinputTotalXiStarCalcMass);
            SimXiStar_70_100->Fill(fTreeVariableMCinputTotalXiStarbarCalcMass);
        }
        
    }
    
 */
        
        
    TFile *fout = TFile::Open(ss+"/Ptbins.root","RECREATE");
    
    for(int ii=0; ii<8; ii++){
        hXiPass[ii]->Write();
        for(int jj=0; jj<14; jj++){
            hXiMass[ii][jj]->Write();
            hXiMassWindow[ii][jj]->Write();
            fXiStar[ii][jj]->Write();
            fXiP[ii][jj]->Write();
            fXiM[ii][jj]->Write();
            fXiMPiM[ii][jj]->Write();
            fXiPPiP[ii][jj]->Write();
            fXiStarPt1[ii][jj]->Write();
            fXiStarPt2[ii][jj]->Write();
            fXiStarPt3[ii][jj]->Write();
            fXiStarPt4[ii][jj]->Write();
            fXiStarPt5[ii][jj]->Write();
            fXiStarPt6[ii][jj]->Write();
            fXiStarPt7[ii][jj]->Write();
            fXiStarPt8[ii][jj]->Write();

        }
    }
/*
        SimXi_all->Write();
        SimXi_0_10->Write();
        SimXi_10_20->Write();
        SimXi_20_30->Write();
        SimXi_30_40->Write();
        SimXi_40_50->Write();
        SimXi_50_60->Write();
        SimXi_60_70->Write();
        SimXi_70_100->Write();
    
        SimXiStar_all->Write();
        SimXiStar_0_10->Write();
        SimXiStar_10_20->Write();
        SimXiStar_20_30->Write();
        SimXiStar_30_40->Write();
        SimXiStar_40_50->Write();
        SimXiStar_50_60->Write();
        SimXiStar_60_70->Write();
        SimXiStar_70_100->Write();
    */
    
    
    evNum->Write();
    hCheckEN->Write();
    
    f->Close();
    fout->Close();
    
    
    
    timer.Stop();
    cout << "Realtime : " << timer.RealTime() << endl;
    cout << "CPUtime : " << timer.CpuTime() << endl;
    
    
    
}
