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
    
    // gSystem->Setenv("alien_CLOSE_SE","ALICE::Trieste::SE");
    
    // Overwrite all generated files, datasets and output results from a previous session
    plugin->SetOverwriteMode(kTRUE);
    // Set the run mode (can be "full", "test", "offline", "submit" or "terminate")
    plugin->SetRunMode("full");  // VERY IMPORTANT
    // Set versions of used packages
    plugin->SetAPIVersion("V1.1x");
    
    // plugin->SetAliROOTVersion("v5-06-11");
    // plugin->SetAliPhysicsVersion("v5-06-11-01");
    
    // there is MB but error on TAttMarker... and so on // ****** For data and MC 206 april 19
    plugin->SetAliROOTVersion("v5-06-15");
    plugin->SetAliPhysicsVersion("v5-06-15-01");
    
    //No MB
    //plugin->SetAliROOTVersion("v5-07-20-1");
    //plugin->SetAliPhysicsVersion("vAN-20160111-1");
    
    
    
    

    
    if(!MCcase) plugin->SetRunPrefix("000");   // real or MC data
    
    
    //TString *Production=new TString("10b");
    //TString *Production=new TString("10c");
    TString *Production=new TString("11h");
    //TString *Production=new TString("10h");
    
    //TString *Production=new TString("10e");
    
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
            plugin->SetGridDataDir("/alice/sim/2014/LHC14a1a"); // LHC14a1b for semi, LHC14a1c for mb
            plugin->SetDataPattern("*AliESDs.root");
            totruns += AddGoodRuns(plugin,"LHC11h_2",MCcase);
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
    //  if(!MCcase) plugin->SetGridWorkingDir("LHC11h_NewCentrality_Sparse-full-1to2-march31");
    if(!MCcase) plugin->SetGridWorkingDir("LHC11h_NewCentrality_Sparse-full-2to2-march31");
    else plugin->SetGridWorkingDir("MC-LHC11h_NewCentrality_Sparse-Central-april20");
    
    // Declare alien output directory. Relative to working directory.
    plugin->SetGridOutputDir("output"); // In this case will be $HOME/work/output
    
    if(!MCcase) plugin->SetMaxMergeFiles(5); // ********* TURN ON
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
    plugin->SetSplitMaxInputFileNumber(0); //50 // 2015Feb
    // Optionally set number of failed jobs that will trigger killing waiting sub-jobs.
    
    // Optionally resubmit threshold.
    plugin->SetMasterResubmitThreshold(95); //97
    // Optionally set time to live (default 30000 sec)
    plugin->SetTTL(60000);
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
    Int_t SetRunNumber = 1;
    
    
    Int_t nruns=0,ngoodruns=0;
    if(lhcPeriod=="LHC11h_2") {
        if(!MCcase){
        if(SetRunNumber==0){
            nruns=18; //total 108
            Int_t runlist[18]={170593, 170572, 170388, 170387, 170315, 170313, 170312, 170311, 170309, 170308, 170306, 170270, 170269, 170268, 170230, 170228, 170207, 170204};
        }
        if(SetRunNumber==1){
            nruns=54; //total 108
            Int_t runlist[54]={170593, 170572, 170388, 170387, 170315, 170313, 170312, 170311, 170309, 170308, 170306, 170270, 170269, 170268, 170230, 170228, 170207, 170204,170203,170193,170163, 170159, 170155, 170091, 170089, 170088, 170085, 170084, 170083, 170081,170040, 170027, 169965, 169923, 169859, 169858,169855, 169846, 169838, 169837, 169835, 169591, 169590, 169588, 169587, 169586, 169557, 169555, 169554, 169553, 169550, 169515, 169512, 169506};
        }
        if(SetRunNumber==2){
            nruns=54;
            Int_t runlist[54]={169504, 169498, 169475, 169420, 169419, 169418, 169417, 169415, 169411, 169238, 169167, 169160, 169156, 169148, 169145, 169144, 169138, 169099,169094, 169091, 169045, 169044, 169040, 169035, 168992, 168988, 168826, 168777, 168514, 168512, 168511, 168467, 168464, 168460, 168458, 168362,168361, 168342, 168341, 168325, 168322, 168311, 168310, 168115, 168108, 168107, 168105, 168076, 168069, 167988, 167987, 167985, 167920, 167915};
        }
        }if(MCcase){
            if(SetRunNumber==0){
                nruns=1; //total 108
                
                Int_t runlist[1]={167915};
            }
            if(SetRunNumber==1){
                nruns=107; //total 108
                
                Int_t runlist[107]={167915, 167920, 167985, 167987, 168069, 168076, 168105, 168107, 168108, 168115, 168310, 168311, 168322, 168325, 168341, 168342, 168361, 168362, 168458, 168460, 168464, 168467, 168511, 168512, 168514, 168777, 168826, 168988, 168992, 169035, 169040, 169044, 169045, 169091, 169094, 169099, 169138, 169144, 169145, 169148, 169156, 169160, 169167, 169238, 169411, 169415, 169417, 169418, 169419, 169420, 169475, 169498, 169504, 169506, 169512, 169515, 169550, 169553, 169554, 169555, 169557, 169586, 169587, 169588, 169590, 169591, 169835, 169837, 169838, 169846, 169855, 169858, 169859, 169923, 169965, 170027, 170040, 170081, 170083, 170084, 170085, 170088, 170089, 170091, 170155, 170159, 170163, 170193, 170203, 170204, 170207, 170228, 170230, 170268, 170269, 170270, 170306, 170308, 170309, 170311, 170312, 170313, 170315, 170387, 170388, 170572, 170593};
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
