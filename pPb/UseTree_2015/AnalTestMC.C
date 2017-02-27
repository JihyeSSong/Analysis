//#include "AnalTree.h"
//#include "AliVEvent.h"

AnalTestMC(){
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
    Float_t fTreeVariableY;
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
    Float_t fTreeVariableCentralityV0A;
    
    Int_t fTreeVariablemyTracks;
    Int_t fTreeVariablexiTracks;
    Int_t fTreeVariableMCTracks;
    
    Int_t totmyTracks = 0;
    Int_t totxiTracks = 0;
    Int_t totMCTracks = 0;
    
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
    
    
    
    Int_t fTreeVariableRunNumber;
    ULong64_t fTreeVariableEventNumber1;
    ULong64_t fTreeVariableEventNumber2;
    ULong64_t fTreeVariableEventNumber3;
    ULong64_t fTreeVariableEventNumber4;

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
    
    TH1D *Ptbin1;
    TH1D *Ptbin2;
    TH1D *Ptbin3;
    TH1D *Ptbin4;
    TH1D *Ptbin5;
    TH1D *Ptbin6;
    TH1D *Ptbin7;
    TH1D *Ptbin8;
    
    TH1D *hXiMC[15];
    TH1D *hXiStarMC[15];
 //   TH1D *fXiStarPta[8];
 //   TH1D *fXiStarPtb[8];
 //   TH1D *fXiStarPtc[8];

    
    TH1D *fXiStarPt1[8][14];
    TH1D *fXiStarPt2[8][14];
    TH1D *fXiStarPt3[8][14];
    TH1D *fXiStarPt4[8][14];
    TH1D *fXiStarPt5[8][14];
    TH1D *fXiStarPt6[8][14];
    TH1D *fXiStarPt7[8][14];
    TH1D *fXiStarPt8[8][14];
    
    for(int iimc = 0; iimc<15; iimc++){
        hXiMC[iimc] = new TH1D(Form("hXiMC_%d",iimc),Form("hXiMC_%d",iimc),200,1.2,1.4);
        hXiStarMC[iimc] = new TH1D(Form("hXiStarMC_%d",iimc),Form("hXiStarMC_%d",iimc),200,1.4,1.6);
        

    }
       
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
    TH3F *fMCinputXiS = new TH3F("fMCinputXiS","Invariant Mass Distribution", 100,0,10, 40,-2,2, 200,1.4,1.6);
    TH3F *fMCinputXiSbar = new TH3F("fMCinputXiSbar","Invariant Mass Distribution", 100,0,10, 40,-2,2, 200,1.4,1.6);
    
    TH3F *fMCoutputXiS = new TH3F("fMCoutputXiS","Invariant Mass Distribution", 100,0,10, 40,-2,2, 200,1.4,1.6);
    TH3F *fMCoutputXiSbar = new TH3F("fMCoutputXiSbar","Invariant Mass Distribution", 100,0,10, 40,-2,2, 200,1.4,1.6);

    
    TH1F *evNum = new TH1F("evNum","event numner in file",1000,0,1000);
    TH1F *hCheckEN = new TH1F("hCheckEN","Checking Event Number",10,0,10);
    
    
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
    fTreeEvent->SetBranchAddress("fTreeVariableCentrality",&fTreeVariableCentrality);
    fTreeEvent->SetBranchAddress("fTreeVariableCentralityV0M",&fTreeVariableCentralityV0M);
    fTreeEvent->SetBranchAddress("fTreeVariableCentralityV0A",&fTreeVariableCentralityV0A);
    fTreeEvent->SetBranchAddress("fTreeVariableRunNumber",&fTreeVariableRunNumber);
    fTreeEvent->SetBranchAddress("fTreeVariableEventNumber1",&fTreeVariableEventNumber1);
    fTreeEvent->SetBranchAddress("fTreeVariableEventNumberInFile",&fTreeVariableEventNumberInFile);
    fTreeEvent->SetBranchAddress("fTreeVariablemyTracks",&fTreeVariablemyTracks);
    fTreeEvent->SetBranchAddress("fTreeVariablexiTracks",&fTreeVariablexiTracks);
    fTreeEvent->SetBranchAddress("fTreeVariableMCTracks",&fTreeVariableMCTracks);
    
    TTree *fTreeXi = (TTree*)f->Get("fTreeXi");
    fTreeXi->SetBranchAddress("fTreeVariableXiMass",&fTreeVariableXiMass);
    fTreeXi->SetBranchAddress("fTreeVariableXiPt",&fTreeVariableXiPt);
    fTreeXi->SetBranchAddress("fTreeVariableY",&fTreeVariableY);
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
    fTreeMC->SetBranchAddress("fTreeVariableEventNumber4",&fTreeVariableEventNumber4);

    double fMaxDecayLength = 100.;
    double raprange1 = -0.5;
    double raprange2 = 0.5;
    double ptedges_Dhevan[9]={0.8, 1.2, 1.6, 2.0, 2.4, 3.2, 4.0, 4.8, 5.6};
    double input_number1 = 0;
    double input_number2 = 0;
    double input_number3 = 0;
    double input_number4 = 0;
    double input_number5 = 0;
    double input_number6 = 0;
    double input_number7 = 0;
    double input_number8 = 0;


   

    
    for (Long64_t ev=0; ev<fTreeEvent->GetEntries(); ev++){
        //cout << "fTreeEvent loop : " << ev ;
        
        fTreeEvent->GetEntry(ev);
        evNum->Fill(fTreeVariableEventNumberInFile);
        //cout << " is loaded" << endl;
        
        totxiTracks = totxiTracks + fTreeVariablexiTracks;
        totmyTracks = totmyTracks + fTreeVariablemyTracks;
        totMCTracks = totMCTracks + fTreeVariableMCTracks;
        
        
        
        for(Long64_t imc=0; imc<fTreeVariableMCTracks; imc++) {
            
            fTreeMC->GetEntry(totMCTracks-fTreeVariableMCTracks+imc);
            
            if(fTreeVariableEventNumber4 != fTreeVariableEventNumber1) hCheckEN->Fill(3);
            //if(fTreeVariableEventNumber4 != fTreeVariableEventNumber1) continue; // event number method
            
            if(fTreeVariableMCinputTotalSongPID==-1) continue;
            
            if(fTreeVariableMCinputTotalSongPID==1) hXiMC[0]->Fill(fTreeVariableMCinputTotalXiCalcMass);
            if(fTreeVariableMCinputTotalSongPID==2) hXiMC[0]->Fill(fTreeVariableMCinputTotalXibarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==3) hXiStarMC[0]->Fill(fTreeVariableMCinputTotalXiStarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==4) hXiStarMC[0]->Fill(fTreeVariableMCinputTotalXiStarbarCalcMass);
            
            if(fTreeVariableMCinputTotalSongPID==3) fMCinputXiS->Fill(fTreeVariableMCinputTotalXiStarPt,fTreeVariableMCinputTotalXiStarY, fTreeVariableMCinputTotalXiStarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==4) fMCinputXiSbar->Fill(fTreeVariableMCinputTotalXiStarbarPt,fTreeVariableMCinputTotalXiStarbarY, fTreeVariableMCinputTotalXiStarbarCalcMass);
            
            
            
            if(fTreeVariableMCinputTotalSongPID==1) hXiMC[1]->Fill(fTreeVariableMCinputTotalXiCalcMass);
            if(fTreeVariableMCinputTotalSongPID==2) hXiMC[1]->Fill(fTreeVariableMCinputTotalXibarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==3) hXiStarMC[1]->Fill(fTreeVariableMCinputTotalXiStarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==4) hXiStarMC[1]->Fill(fTreeVariableMCinputTotalXiStarbarCalcMass);
            
            if(fTreeVariableNclusp<fCutValues[0][0]) continue;
            
            if(fTreeVariableMCinputTotalSongPID==1) hXiMC[2]->Fill(fTreeVariableMCinputTotalXiCalcMass);
            if(fTreeVariableMCinputTotalSongPID==2) hXiMC[2]->Fill(fTreeVariableMCinputTotalXibarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==3) hXiStarMC[2]->Fill(fTreeVariableMCinputTotalXiStarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==4) hXiStarMC[2]->Fill(fTreeVariableMCinputTotalXiStarbarCalcMass);
            
            
            if(fTreeVariableNcluspi1<fCutValues[0][1]) continue;
            
            if(fTreeVariableMCinputTotalSongPID==1) hXiMC[3]->Fill(fTreeVariableMCinputTotalXiCalcMass);
            if(fTreeVariableMCinputTotalSongPID==2) hXiMC[3]->Fill(fTreeVariableMCinputTotalXibarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==3) hXiStarMC[3]->Fill(fTreeVariableMCinputTotalXiStarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==4) hXiStarMC[3]->Fill(fTreeVariableMCinputTotalXiStarbarCalcMass);
            
            
            if(fTreeVariableNcluspi2<fCutValues[0][2]) continue;
            
            if(fTreeVariableMCinputTotalSongPID==1) hXiMC[4]->Fill(fTreeVariableMCinputTotalXiCalcMass);
            if(fTreeVariableMCinputTotalSongPID==2) hXiMC[4]->Fill(fTreeVariableMCinputTotalXibarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==3) hXiStarMC[4]->Fill(fTreeVariableMCinputTotalXiStarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==4) hXiStarMC[4]->Fill(fTreeVariableMCinputTotalXiStarbarCalcMass);
            
            if(fTreeVariablePVdcaP<fCutValues[0][4]) continue;
            
            if(fTreeVariableMCinputTotalSongPID==1) hXiMC[5]->Fill(fTreeVariableMCinputTotalXiCalcMass);
            if(fTreeVariableMCinputTotalSongPID==2) hXiMC[5]->Fill(fTreeVariableMCinputTotalXibarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==3) hXiStarMC[5]->Fill(fTreeVariableMCinputTotalXiStarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==4) hXiStarMC[5]->Fill(fTreeVariableMCinputTotalXiStarbarCalcMass);
            
            if(fTreeVariablePVdcaPi1<fCutValues[0][5])continue;
            
            if(fTreeVariableMCinputTotalSongPID==1) hXiMC[6]->Fill(fTreeVariableMCinputTotalXiCalcMass);
            if(fTreeVariableMCinputTotalSongPID==2) hXiMC[6]->Fill(fTreeVariableMCinputTotalXibarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==3) hXiStarMC[6]->Fill(fTreeVariableMCinputTotalXiStarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==4) hXiStarMC[6]->Fill(fTreeVariableMCinputTotalXiStarbarCalcMass);
            
            if(fTreeVariablePVdcaPi2<fCutValues[0][6]) continue;
            
            if(fTreeVariableMCinputTotalSongPID==1) hXiMC[7]->Fill(fTreeVariableMCinputTotalXiCalcMass);
            if(fTreeVariableMCinputTotalSongPID==2) hXiMC[7]->Fill(fTreeVariableMCinputTotalXibarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==3) hXiStarMC[7]->Fill(fTreeVariableMCinputTotalXiStarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==4) hXiStarMC[7]->Fill(fTreeVariableMCinputTotalXiStarbarCalcMass);
            
            
            if(fTreeVariablePVdcaLambda<fCutValues[0][7]) continue;
            
            if(fTreeVariableMCinputTotalSongPID==1) hXiMC[8]->Fill(fTreeVariableMCinputTotalXiCalcMass);
            if(fTreeVariableMCinputTotalSongPID==2) hXiMC[8]->Fill(fTreeVariableMCinputTotalXibarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==3) hXiStarMC[8]->Fill(fTreeVariableMCinputTotalXiStarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==4) hXiStarMC[8]->Fill(fTreeVariableMCinputTotalXiStarbarCalcMass);
            
            
            if(fTreeVariableDCApip>fCutValues[0][9])continue;
            
            if(fTreeVariableMCinputTotalSongPID==1) hXiMC[9]->Fill(fTreeVariableMCinputTotalXiCalcMass);
            if(fTreeVariableMCinputTotalSongPID==2) hXiMC[9]->Fill(fTreeVariableMCinputTotalXibarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==3) hXiStarMC[9]->Fill(fTreeVariableMCinputTotalXiStarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==4) hXiStarMC[9]->Fill(fTreeVariableMCinputTotalXiStarbarCalcMass);
            
            if(fTreeVariableDCApilambda>fCutValues[0][10])continue;
            
            if(fTreeVariableMCinputTotalSongPID==1) hXiMC[10]->Fill(fTreeVariableMCinputTotalXiCalcMass);
            if(fTreeVariableMCinputTotalSongPID==2) hXiMC[10]->Fill(fTreeVariableMCinputTotalXibarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==3) hXiStarMC[10]->Fill(fTreeVariableMCinputTotalXiStarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==4) hXiStarMC[10]->Fill(fTreeVariableMCinputTotalXiStarbarCalcMass);
            
            if(fTreeVariableDecayXYlambda<fCutValues[0][11])continue;
            
            if(fTreeVariableMCinputTotalSongPID==1) hXiMC[11]->Fill(fTreeVariableMCinputTotalXiCalcMass);
            if(fTreeVariableMCinputTotalSongPID==2) hXiMC[11]->Fill(fTreeVariableMCinputTotalXibarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==3) hXiStarMC[11]->Fill(fTreeVariableMCinputTotalXiStarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==4) hXiStarMC[11]->Fill(fTreeVariableMCinputTotalXiStarbarCalcMass);
            
            if(fTreeVariableDecayXYxi<fCutValues[0][12]) continue;
            
            if(fTreeVariableMCinputTotalSongPID==1) hXiMC[12]->Fill(fTreeVariableMCinputTotalXiCalcMass);
            if(fTreeVariableMCinputTotalSongPID==2) hXiMC[12]->Fill(fTreeVariableMCinputTotalXibarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==3) hXiStarMC[12]->Fill(fTreeVariableMCinputTotalXiStarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==4) hXiStarMC[12]->Fill(fTreeVariableMCinputTotalXiStarbarCalcMass);
            
            if(fTreevariableCosPAlambda<fCutValues[0][13]) continue;
            
            if(fTreeVariableMCinputTotalSongPID==1) hXiMC[13]->Fill(fTreeVariableMCinputTotalXiCalcMass);
            if(fTreeVariableMCinputTotalSongPID==2) hXiMC[13]->Fill(fTreeVariableMCinputTotalXibarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==3) hXiStarMC[13]->Fill(fTreeVariableMCinputTotalXiStarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==4) hXiStarMC[13]->Fill(fTreeVariableMCinputTotalXiStarbarCalcMass);
            
            if(fTreevariableCosPAxi<fCutValues[0][14]) continue;
            
            if(fTreeVariableMCinputTotalSongPID==1) hXiMC[14]->Fill(fTreeVariableMCinputTotalXiCalcMass);
            if(fTreeVariableMCinputTotalSongPID==2) hXiMC[14]->Fill(fTreeVariableMCinputTotalXibarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==3) hXiStarMC[14]->Fill(fTreeVariableMCinputTotalXiStarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==4) hXiStarMC[14]->Fill(fTreeVariableMCinputTotalXiStarbarCalcMass);
            
            
            if(fTreeVariableMCinputTotalSongPID==3) fMCoutputXiS->Fill(fTreeVariableMCinputTotalXiStarPt,fTreeVariableMCinputTotalXiStarY, fTreeVariableMCinputTotalXiStarCalcMass);
            if(fTreeVariableMCinputTotalSongPID==4) fMCoutputXiSbar->Fill(fTreeVariableMCinputTotalXiStarbarPt,fTreeVariableMCinputTotalXiStarbarY, fTreeVariableMCinputTotalXiStarbarCalcMass);
            
            
            
        }

        
        
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
            
            
            if(fTreeVariableEventNumber2 != fTreeVariableEventNumber1) hCheckEN->Fill(1);
            if(fTreeVariableEventNumber2 != fTreeVariableEventNumber1) continue; // event number method
            
            
            if(fTreeVariableXiPt<.15||fTreeVariableXiPt>1000) continue;
            
            nc = 0;
            
            
            
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
            
         
            for(int jj=0; jj<14; jj++) {
                if(jj>nc) continue;
                hXiPass[ct]->Fill(jj);
            }
            
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
          //  if(fTreeVariableXiMass-fTrueMassXi>0.01||fTreeVariableXiMass-fTrueMassXi<-0.01) continue;
 
            
            //     for(int ii=0; ii<4; ii++){
            //         if(ii>ct) continue;
            for(int jj=0; jj<14; jj++){
                if(jj>nc) continue;
                hXiMassWindow[ct][jj]->Fill(fTreeVariableXiMass);
                
            }
            
            
            //      }
            
  
            
            
        } // Xi loop
    
 
        if (fTreeEvent->GetEntries()%30 == 0) cout << "Processing : " << ev << "/" << fTreeEvent->GetEntries() << endl;
        
    } // Base loop
    
    
        
    TFile *fout = TFile::Open(ss+"/MCtest.root","RECREATE");

    
    fMCinputXiS->Write();
    fMCinputXiSbar->Write();
    fMCoutputXiS->Write();
    fMCoutputXiSbar->Write();

    for(int jmc=0; jmc<15; jmc++){
        
        hXiMC[jmc]->Write();
        hXiStarMC[jmc]->Write();
    }


/*
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

*/
    
    evNum->Write();
    hCheckEN->Write();
    
    f->Close();
    fout->Close();
    
    
    
    timer.Stop();
    cout << "Realtime : " << timer.RealTime() << endl;
    cout << "CPUtime : " << timer.CpuTime() << endl;
    
    
    
}
