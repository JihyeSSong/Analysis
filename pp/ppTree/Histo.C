Histo(){
    for (Int_t k=0; k<1 ; k++ ) {
        Anal(k);
    }
  
}

Anal(Int_t split=-1){
 
    
    TStopwatch timer;
    timer.Start();
    // TFile *f = new TFile("MyOutput.root","READ");
    // cout << "open root file " << endl;
    
    
    
    TString ss = Form("%03d",split); //d -> 03d
    cout << ss << endl;
    
    
    TFile *f = TFile::Open(ss+"/MyOutput.root","READ");
    if(f==NULL) continue; // this is for empty directory
    
    cout << "open root file : " << ss << "/MyOutput.root"<< endl;

    
    TList* list = f->Get("MyList");
    TH3 *hXi;
    TH3 *hXibar;
    TH3 *hXiStar;
    TH3 *hXiStarbar;
    
    TH1 *Xi;
    TH1 *Xibar;
    TH1F *tot_Xi = new TH1F("tot_Xi","Xi distribution 0 - 30",200,1.2,1.4);
    TH1 *XiStar;
    TH1 *XiStarbar;
    TH1F *tot_XiStar = new TH1F("tot_XiStar","XiStar distribution 0 - 30",200,1.4,1.6);
    
    TH2 *PID;
    TH2 *PID_pr;
    TH2 *PID_k;
    TH2 *PID_pi;
    
    TH2 *dEdx1;
    TH2 *dEdx2;
    TH2 *dEdx3;
    TH2 *dEdx11;
    TH2 *dEdx22;
    TH2 *dEdx33;
    TH2 *dEdxPi;

    
    
    hXi = (TH3*)list->FindObject("fXi_0");
    Xi = hXi->Project3D("z");
    hXibar = (TH3*)list->FindObject("fXibar_0");
    Xibar = hXibar->Project3D("z");
    
    hXiStar = (TH3*)list->FindObject("fXiMinusPiPlus_0");
    XiStar = hXiStar->Project3D("z");
    hXiStarbar = (TH3*)list->FindObject("fXiPlusPiMinus_0");
    XiStarbar = hXiStarbar->Project3D("z");
    
    tot_Xi->Add(Xi);
    tot_Xi->Add(Xibar);
    tot_XiStar->Add(XiStar);
    tot_XiStar->Add(XiStarbar);
    
    PID = (TH2*)list->FindObject("TPCPID");
    PID_pr = (TH2*)list->FindObject("TPCPID3");
    PID_k = (TH2*)list->FindObject("TPCPID2");
    PID_pi = (TH2*)list->FindObject("TPCPID1");
    
    dEdx1 = (TH2*)list->FindObject("XidEdx1");
    dEdx2 = (TH2*)list->FindObject("XidEdx2");
    dEdx3 = (TH2*)list->FindObject("XidEdx3");
    dEdx11 = (TH2*)list->FindObject("XidEdx11");
    dEdx22 = (TH2*)list->FindObject("XidEdx22");
    dEdx33 = (TH2*)list->FindObject("XidEdx33");

    
    
    
    TFile *fout = TFile::Open(ss+"/test.root","RECREATE");

    Xi->Write();
    Xibar->Write();
    XiStar->Write();
    XiStarbar->Write();
    tot_Xi->Write();
    tot_XiStar->Write();
    
    PID ->Write();
    PID_pr->Write();
    PID_k->Write();
    PID_pi->Write();
    
    dEdx1->Write();
    dEdx2->Write();
    dEdx3->Write();
    dEdx11->Write();
    dEdx22->Write();
    dEdx33->Write();
    
    
    f->Close();
    fout->Close();
    
    
    
    timer.Stop();
    cout << "Realtime : " << timer.RealTime() << endl;
    cout << "CPUtime : " << timer.CpuTime() << endl;

    
    
}