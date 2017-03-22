AliAnalysisGrid* CreateAlienHandler(bool MCcase)
{
    // Check if user has a valid token, otherwise make one. This has limitations.
    // One can always follow the standard procedure of calling alien-token-init then
    //   source /tmp/gclient_env_$UID in the current shell.
    //   if (!AliAnalysisGrid::CreateToken()) return NULL;
    
    AliAnalysisAlien *plugin = new AliAnalysisAlien();
    
    //  plugin->SetUser("Your alien username here");
    plugin->SetUser("jisong");
    
    gSystem->Setenv("alien_CLOSE_SE","working_disk_SE");
    
   //  gSystem->Setenv("alien_CLOSE_SE","ALICE::ORNL::LCG");
    
    // Overwrite all generated files, datasets and output results from a previous session
    plugin->SetOverwriteMode(kTRUE);
    // Set the run mode (can be "full", "test", "offline", "submit" or "terminate")
    plugin->SetRunMode("terminate");  // VERY IMPORTANT
    // Set versions of used packages
    plugin->SetAPIVersion("V1.1x");
    
    // plugin->SetAliROOTVersion("v5-06-11");
    // plugin->SetAliPhysicsVersion("v5-06-11-01");
    
    // there is MB but error on TAttMarker... and so on here i have kMB // !!!USED !!!!!!!!
    plugin->SetAliROOTVersion("v5-06-15");
    plugin->SetAliPhysicsVersion("v5-06-15-01"); //work for kMB untill June 23 2016.  //for data
    
    
    
    // below is test
  //   plugin->SetROOTVersion("v5-34-30-alice-24");
  //   plugin->SetAliROOTVersion("v5-08-12-1");
  //   plugin->SetAliPhysicsVersion("vAN-20160616-1");
    
    
    
    if(!MCcase) plugin->SetRunPrefix("000");   // real or MC data
    
    
    TString *Production=new TString("11h");
    //TString *Production=new TString("10h");
    
    
    Int_t totruns=0;
    // Declare input data to be processed.
    if(!MCcase){// Real data
        if(Production->Contains("10h")){// ESDs
            plugin->SetGridDataDir("/alice/data/2010/LHC10h");
            plugin->SetDataPattern("ESDs/pass2/*AliESDs.root");
            totruns += AddGoodRuns(plugin,"LHC10h");
        }
        
        if(Production->Contains("11h")){// ESDs
            plugin->SetGridDataDir("/alice/data/2011/LHC11h_2");
            plugin->SetDataPattern("ESDs/pass2/*AliESDs.root");
            totruns += AddGoodRuns(plugin,"LHC11h_2");
        }
        if(Production->Contains("10d")){// ESDs
            plugin->SetGridDataDir("/alice/data/2010/LHC10d");
            plugin->SetDataPattern("ESDs/pass2/*AliESDs.root");
            totruns += AddGoodRuns(plugin,"LHC10d");
        }
        if(Production->Contains("10e")){// ESDs
            plugin->SetGridDataDir("/alice/data/2010/LHC10e");
            plugin->SetDataPattern("ESDs/pass2/*AliESDs.root");
            totruns += AddGoodRuns(plugin,"LHC10e");
        }
    }else {// MC data
        if(Production->Contains("11h")){// AODs
            plugin->SetGridDataDir("/alice/sim/2014/LHC14a1c"); // LHC14a1b for semi, LHC14a1c for mb
            plugin->SetDataPattern("*AliESDs.root");
            totruns += AddGoodRuns(plugin,"LHC11h_2",MCcase);
        }
        if(Production->Contains("10d")){// AODs
            plugin->SetGridDataDir("/alice/sim/LHC10f6");
            plugin->SetDataPattern("*AliESDs.root");
            totruns += AddGoodRuns(plugin,"LHC10d",MCcase);
        }
        if(Production->Contains("10h")){// ESDs
            plugin->SetGridDataDir("/alice/sim/LHC11a10a_bis");
            plugin->SetDataPattern("*AliESDs.root");
            totruns += AddGoodRuns(plugin,"LHC10h",MCcase);
        }
        
        
        if(Production->Contains("10d")){// David's Xi MC data set
            plugin->SetGridDataDir("/alice/sim/LHC11a6a");
            plugin->SetDataPattern("*AliESDs.root");
            totruns += AddGoodRuns(plugin,"LHC10d",MCcase);
        }
        if(Production->Contains("10e")){// AODs
            plugin->SetGridDataDir("/alice/sim/LHC10e20");
            plugin->SetDataPattern("*AliESDs.root");
            totruns += AddGoodRuns(plugin,"LHC10e",MCcase);
        }
    }
    
    
    
    // Define alien work directory where all files will be copied. Relative to aliBn $HOME.

    if(!MCcase) plugin->SetGridWorkingDir("LHC11h_NewCentrality_JULY8_Set6To6_Signal-V9");
//    if(!MCcase) plugin->SetGridWorkingDir("LHC11h_NewCentrality_JULY8_Set1To6_Signal-DEF");

    else plugin->SetGridWorkingDir("LHC11h_MC_kCent_LHC14a1c_JULY8-Set3to3-MW6");

    
    // Declare alien output directory. Relative to working directory.
    plugin->SetGridOutputDir("output"); // In this case will be $HOME/work/output
    if(!MCcase) plugin->SetMaxMergeFiles(20); // ********* TURN ON for data
    plugin->SetOneStageMerging(kFALSE);
    plugin->SetMaxMergeStages(2);
    plugin->SetMergeViaJDL();
    //plugin->SetUseSubmitPolicy();
    
    // Declare the analysis source files names separated by blancs. To be compiled runtime
    // using ACLiC on the worker nodes.
    plugin->SetAnalysisSource("AliXiStarPbPbEventCollection.cxx AliXiStarPbPb.cxx");
    // Declare all libraries (other than the default ones for the framework. These will be
    // loaded by the generated analysis macro. Add all extra files (task .cxx/.h) here.
    plugin->SetAdditionalLibs("AliXiStarPbPbEventCollection.h AliXiStarPbPbEventCollection.cxx AliXiStarPbPb.h AliXiStarPbPb.cxx");
    
    // No need for output file names. Procedure is automatic.
    plugin->SetDefaultOutputs(kTRUE);
    //plugin->SetOutputFiles("MyOutput.root");
    // No need define the files to be archived. Note that this is handled automatically by the plugin.
    //plugin->SetOutputArchive("log_archive.zip:stdout,stderr");
    // Set a name for the generated analysis macro (default MyAnalysis.C) Make this unique !
    plugin->SetAnalysisMacro("GeneratedMacro.C");
    // Optionally set maximum number of input files/subjob (default 100, put 0 to ignore). The optimum for an analysis
    // is correlated with the run time - count few hours TTL per job, not minutes !
    if(!MCcase)plugin->SetSplitMaxInputFileNumber(10);// // TURN ON for data//if(!MCcase)
    else plugin->SetSplitMaxInputFileNumber(0); //50 // MC
    
    
    // Optionally set number of failed jobs that will trigger killing waiting sub-jobs.
    
    // Optionally resubmit threshold.
    plugin->SetMasterResubmitThreshold(95); //97
    // Optionally set time to live (default 30000 sec)
    plugin->SetTTL(60000); //May19 for SIGNAL!!!
    
    // Optionally set input format (default xml-single)
    plugin->SetInputFormat("xml-single");
    // Optionally modify the name of the generated JDL (default analysis.jdl)
    plugin->SetJDLName("runCode.jdl");
    // Optionally modify job price (default 1)
    plugin->SetPrice(1);
    // Optionally modify split mode (default 'se')
    plugin->SetSplitMode("se");
    
    return plugin;
    
}
//___________________________________________________________________-
Int_t AddGoodRuns(AliAnalysisAlien* plugin,TString lhcPeriod,bool MCcase=kFALSE) {
    //
    // Adds good runs from the Monalisa Run Condition Table
    Int_t SetRunNumber = 6;
      Int_t nruns=0,ngoodruns=0;
   
  
    if(lhcPeriod=="LHC11h_2") {
        if(!MCcase){
            if(SetRunNumber==0){
                nruns=1; //total 108
                Int_t runlist[1]={168115};// //170593
            }
            if(SetRunNumber==1){
                nruns=18; //total 108
                Int_t runlist[18]={170593, 170572, 170388, 170387, 170315, 170313, 170312, 170311, 170309, 170308, 170306, 170270, 170269, 170268, 170230, 170228, 170207, 170204};
            }
            if(SetRunNumber==2){
                nruns=18;
                Int_t runlist[18]={170203, 170193,170163, 170159, 170155, 170091, 170089, 170088, 170085, 170084, 170083, 170081,170040, 170027, 169965, 169923, 169859, 169858
                };
            }
            if(SetRunNumber==3){
                nruns=18; //total 108
                Int_t runlist[18]={169855, 169846, 169838, 169837, 169835, 169591, 169590, 169588, 169587, 169586, 169557, 169555, 169554, 169553, 169550, 169515, 169512, 169506};
            }if(SetRunNumber==4){
                nruns=18; //total 108
                Int_t runlist[18]={169504, 169498, 169475, 169420, 169419, 169418, 169417, 169415, 169411, 169238, 169167, 169160, 169156, 169148, 169145, 169144,169138, 169099};
            }
            if(SetRunNumber==5){
                nruns=18;
                Int_t runlist[18]={169094, 169091, 169045, 169044, 169040, 169035, 168992, 168988, 168826, 168777, 168514, 168512, 168511, 168467, 168464, 168460, 168458, 168362};
            }
            if(SetRunNumber==6){
                nruns=18;
                Int_t runlist[18]={168361, 168342, 168341, 168325, 168322, 168311, 168310, 168115, 168108, 168107, 168105, 168076, 168069, 167988, 167987, 167985, 167920, 167915};
            }
            
        }
        if(MCcase){
            if(SetRunNumber==0){
                 nruns=107; //total 108
                
                Int_t runlist[107]={167915, 167920, 167985, 167987, 168069, 168076, 168105, 168107, 168108, 168115, 168310, 168311, 168322, 168325, 168341, 168342, 168361, 168362, 168458, 168460, 168464, 168467, 168511, 168512, 168514, 168777, 168826, 168988, 168992, 169035, 169040, 169044, 169045, 169091, 169094, 169099, 169138, 169144, 169145, 169148, 169156, 169160, 169167, 169238, 169411, 169415, 169417, 169418, 169419, 169420, 169475, 169498, 169504, 169506, 169512, 169515, 169550, 169553, 169554, 169555, 169557, 169586, 169587, 169588, 169590, 169591, 169835, 169837, 169838, 169846, 169855, 169858, 169859, 169923, 169965, 170027, 170040, 170081, 170083, 170084, 170085, 170088, 170089, 170091, 170155, 170159, 170163, 170193, 170203, 170204, 170207, 170228, 170230, 170268, 170269, 170270, 170306, 170308, 170309, 170311, 170312, 170313, 170315, 170387, 170388, 170572, 170593};
            }
            if(SetRunNumber==1){
                nruns=36; //total 108
                
                Int_t runlist[36]={167915, 167920, 167985, 167987, 168069, 168076, 168105, 168107, 168108, 168115, 168310, 168311, 168322, 168325, 168341, 168342, 168361, 168362, 168458, 168460, 168464, 168467, 168511, 168512, 168514, 168777, 168826, 168988, 168992, 169035, 169040, 169044, 169045, 169091, 169094, 169099};            }
            if(SetRunNumber==2){
                nruns=36; //total 108
                
                Int_t runlist[36]={ 169138, 169144, 169145, 169148, 169156, 169160, 169167, 169238, 169411, 169415, 169417, 169418, 169419, 169420, 169475, 169498, 169504, 169506, 169512, 169515, 169550, 169553, 169554, 169555, 169557, 169586, 169587, 169588, 169590, 169591, 169835, 169837, 169838, 169846, 169855, 169858};
            }
            if(SetRunNumber==3){
                nruns=35; //total 108
                
                Int_t runlist[35]={ 169859, 169923, 169965, 170027, 170040, 170081, 170083, 170084, 170085, 170088, 170089, 170091, 170155, 170159, 170163, 170193, 170203, 170204, 170207, 170228, 170230, 170268, 170269, 170270, 170306, 170308, 170309, 170311, 170312, 170313, 170315, 170387, 170388, 170572, 170593};
            }

            
            
            
        }
        
        
        
        //170593, 170572, 170388, 170387, 170315, 170313, 170312, 170311, 170309, 170308,
        //170306, 170270, 170269, 170268, 170230, 170228, 170207, 170204, //set1
        
        //170203,170193,170163, 170159, 170155, 170091, 170089, 170088, 170085, 170084,
        // 170083, 170081,170040, 170027, 169965, 169923, 169859, 169858, //set2
        
        //169855, 169846, 169838, 169837, 169835, 169591, 169590, 169588, 169587, 169586,
        //169557, 169555, 169554, 169553, 169550, 169515, 169512, 169506, //set3
        
        //169504, 169498, 169475, 169420, 169419, 169418, 169417, 169415, 169411, 169238,
        //169167, 169160, 169156, 169148, 169145, 169144, 169138, 169099, //set4
        
        //169094, 169091, 169045, 169044, 169040, 169035, 168992, 168988, 168826, 168777,
        //168514, 168512, 168511, 168467, 168464, 168460, 168458, 168362, //set5
        
        //168361, 168342, 168341, 168325, 168322, 168311, 168310, 168115, 168108, 168107,
        //168105, 168076, 168069, 167988, 167987, 167985, 167920, 167915 //set6
        
        
        
        for(Int_t k=0;k<nruns;k++){
            if(MCcase) {
                TString *MCRunName = new TString();
                *MCRunName += runlist[k];
                plugin->AddRunNumber(MCRunName->Data());
            }
            else plugin->AddRunNumber(runlist[k]);
            
            ngoodruns++;
        }
        plugin->SetNrunsPerMaster(1);
    }
    
    
    if(lhcPeriod=="LHC10h") {
        if(!MCcase){
            if(SetRunNumber==1){
                nruns=92; //total 92
                Int_t runlist[92]={139510, 139507, 139505, 139503, 139465, 139438, 139437, 139360, 139329, 139328, 139314, 139310, 139309, 139173, 139107, 139105, 139038, 139037, 139036, 139029, 139028, 138872, 138871, 138870, 138837, 138732, 138730, 138666, 138662, 138653, 138652, 138638, 138624, 138621, 138583, 138582, 138578, 138534, 138469, 138442, 138439, 138438, 138396, 138364, 138275, 138225, 138201, 138197, 138192, 138190, 137848, 137844, 137752, 137751, 137724, 137722, 137718, 137704, 137693, 137692, 137691, 137686, 137685, 137639, 137638, 137608, 137595, 137549, 137546, 137544, 137541, 137539, 137531, 137530, 137443, 137441, 137440, 137439, 137434, 137432, 137431, 137430, 137366, 137243, 137236, 137235, 137232, 137231, 137230, 137162, 137161, 137135};
            }
            
        }
        if(MCcase){
            if(SetRunNumber==1){
                nruns=155; //total 155
                
                Int_t runlist[155]={137124, 137125, 137132, 137133, 137135, 137136, 137137, 137161, 137162, 137163, 137165, 137230, 137231, 137232, 137235, 137236, 137243, 137365, 137366, 137370, 137430, 137431, 137432, 137434, 137439, 137440, 137441, 137443, 137530, 137531, 137539, 137541, 137544, 137546, 137549, 137595, 137608, 137609, 137638, 137639, 137685, 137686, 137689, 137691, 137692, 137693, 137704, 137718, 137722, 137724, 137748, 137751, 137752, 137843, 137844, 137847, 137848, 138125, 138126, 138150, 138151, 138153, 138154, 138190, 138192, 138197, 138200, 138201, 138225, 138275, 138359, 138364, 138396, 138438, 138439, 138442, 138469, 138533, 138534, 138578, 138579, 138582, 138583, 138620, 138621, 138624, 138637, 138638, 138652, 138653, 138662, 138666, 138730, 138731, 138732, 138736, 138737, 138740, 138742, 138795, 138796, 138826, 138828, 138830, 138831, 138836, 138837, 138870, 138871, 138872, 139024, 139025, 139028, 139029, 139030, 139031, 139034, 139036, 139037, 139038, 139042, 139104, 139105, 139107, 139110, 139172, 139173, 139308, 139309, 139310, 139311, 139314, 139316, 139328, 139329, 139360, 139437, 139438, 139439, 139440, 139441, 139465, 139466, 139467, 139470, 139471, 139503, 139504, 139505, 139507, 139510, 139511, 139513, 139514, 13951};
            }
            
            
            
        }
        
        
        
        
        
        for(Int_t k=0;k<nruns;k++){
            if(MCcase) {
                TString *MCRunName = new TString();
                *MCRunName += runlist[k];
                plugin->AddRunNumber(MCRunName->Data());
            }
            else plugin->AddRunNumber(runlist[k]);
            
            ngoodruns++;
        }
        plugin->SetNrunsPerMaster(1);
    }
    

    
    return ngoodruns;
    
}
