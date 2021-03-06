AliAnalysisGrid* CreateAlienHandler(bool MCcase)
{
    // Check if user has a valid token, otherwise make one. This has limitations.
    // One can always follow the standard procedure of calling alien-token-init then
    //   source /tmp/gclient_env_$UID in the current shell.
    //   if (!AliAnalysisGrid::CreateToken()) return NULL;
    
    AliAnalysisAlien *plugin = new AliAnalysisAlien();
    
    //  plugin->SetUser("Your alien username here");
    plugin->SetUser("jisong");
    
    
    
    // Overwrite all generated files, datasets and output results from a previous session
    plugin->SetOverwriteMode(kTRUE);
    // Set the run mode (can be "full", "test", "offline", "submit" or "terminate")
    plugin->SetRunMode("full");  // VERY IMPORTANT
    // Set versions of used packages
    plugin->SetAPIVersion("V1.1x");
    //    plugin->SetROOTVersion("v5-34-05");
    //    plugin->SetAliROOTVersion("v5-04-80-AN");
    
    plugin->SetROOTVersion("v5-34-08-6");
    plugin->SetAliROOTVersion("vAN-20141030");
    
    
    if(!MCcase) plugin->SetRunPrefix("000");   // real or MC data
    
    
    //TString *Production=new TString("10b");
    //TString *Production=new TString("10c");
    
    TString *Production=new TString("13c");
    
    //TString *Production=new TString("10h");
    
    //TString *Production=new TString("10e");
    
    Int_t totruns=0;
    // Declare input data to be processed.
    if(!MCcase){// Real data
        /*if(Production->Contains("10b")){// AODs
         plugin->SetGridDataDir("/alice/data/2010/LHC10b");
         plugin->SetDataPattern("ESDs/pass2/AOD038/*AliAOD.root");
         totruns += AddGoodRuns(plugin,"LHC10b");
         }*/
        
        if(Production->Contains("10h")){// ESDs
            plugin->SetGridDataDir("/alice/data/2010/LHC10h");
            plugin->SetDataPattern("ESDs/pass2/*AliESDs.root");
            totruns += AddGoodRuns(plugin,"LHC10h");
        }
        /*
         if(Production->Contains("10c")){// AODs
         plugin->SetGridDataDir("/alice/data/2010/LHC10c");
         plugin->SetDataPattern("ESDs/pass2/AOD038/*AliAOD.root");
         totruns += AddGoodRuns(plugin,"LHC10c");
         }*/
        if(Production->Contains("13b")){// ESDs
            plugin->SetGridDataDir("/alice/data/2013/LHC13b");
            plugin->SetDataPattern("ESDs/pass3/*AliESDs.root");
            totruns += AddGoodRuns(plugin,"LHC13b");
        }
        /*
         if(Production->Contains("10d")){// AODs
         plugin->SetGridDataDir("/alice/data/2010/LHC10d");
         plugin->SetDataPattern("ESDs/pass2/AOD057/*AliAOD.root");
         totruns += AddGoodRuns(plugin,"LHC10d");
         }*/
        
        if(Production->Contains("13c")){// ESDs
            plugin->SetGridDataDir("/alice/data/2013/LHC13c");
            plugin->SetDataPattern("pass2/*AliESDs.root");
            totruns += AddGoodRuns(plugin,"LHC13c");
        }
        
        /*
         if(Production->Contains("10e")){// AODs
         plugin->SetGridDataDir("/alice/data/2010/LHC10e");
         plugin->SetDataPattern("ESDs/pass2/AOD057/*AliAOD.root");
         totruns += AddGoodRuns(plugin,"LHC10e");
         }*/
        if(Production->Contains("10e")){// ESDs
            plugin->SetGridDataDir("/alice/data/2010/LHC10e");
            plugin->SetDataPattern("ESDs/pass2/*AliESDs.root");
            totruns += AddGoodRuns(plugin,"LHC10e");
        }
    }else {// MC data
        if(Production->Contains("13c")){// AODs
            plugin->SetGridDataDir("/alice/sim/2013/LHC13b2_efix_p4");
            plugin->SetDataPattern("*AliESDs.root");
            totruns += AddGoodRuns(plugin,"LHC13c",MCcase);
        }/*
          if(Production->Contains("10b")){// AODs
          plugin->SetGridDataDir("/alice/sim/LHC10d1");
          plugin->SetDataPattern("AOD056/*AliAOD.root");
          totruns += AddGoodRuns(plugin,"LHC10b",MCcase);
          }*/
        /*
         if(Production->Contains("10c")){// AODs
         plugin->SetGridDataDir("/alice/sim/LHC10d4");
         plugin->SetDataPattern("AOD056/*AliAOD.root");
         totruns += AddGoodRuns(plugin,"LHC10c",MCcase);
         }*/
        if(Production->Contains("10h")){// ESDs
            plugin->SetGridDataDir("/alice/sim/LHC11a10a_bis");
            plugin->SetDataPattern("*AliESDs.root");
            totruns += AddGoodRuns(plugin,"LHC10h",MCcase);
        }
        
        if(Production->Contains("10e")){// AODs
            plugin->SetGridDataDir("/alice/sim/LHC10e20");
            plugin->SetDataPattern("*AliESDs.root");
            totruns += AddGoodRuns(plugin,"LHC10e",MCcase);
        }
    }
    
    
    
    //plugin->AddRunNumber(117052); // an LHC10b run
    //plugin->AddRunNumber(120822); // an LHC10c run
    //plugin->AddRunNumber(120820); // an LHC10c run
    //plugin->AddRunNumber(137366);// a PbPb run
    
   // plugin->AddRunNumber(195531); // an LHC10b run

    
    // Define alien work directory where all files will be copied. Relative to alien $HOME.
  // if(!MCcase) plugin->SetGridWorkingDir("LHC13c_CompareXi_ANtoMe");
    
  //  if(!MCcase) plugin->SetGridWorkingDir("LHC13c_CompareXi_pp");
    if(!MCcase) plugin->SetGridWorkingDir("LHC13c_testestest");

    
    else plugin->SetGridWorkingDir("LHC13b2_efix_p4_nov13");
    
    // Declare alien output directory. Relative to working directory.
    plugin->SetGridOutputDir("output"); // In this case will be $HOME/work/output
    //plugin->SetMaxMergeFiles(50);
    plugin->SetOneStageMerging(kFALSE);
    plugin->SetMaxMergeStages(2);
    plugin->SetMergeViaJDL();
    //plugin->SetUseSubmitPolicy();
    
    // Declare the analysis source files names separated by blancs. To be compiled runtime
    // using ACLiC on the worker nodes.
    plugin->SetAnalysisSource("AliXiStarEventCollection.cxx AliXiStar.cxx");
    // Declare all libraries (other than the default ones for the framework. These will be
    // loaded by the generated analysis macro. Add all extra files (task .cxx/.h) here.
    plugin->SetAdditionalLibs("AliXiStarEventCollection.h AliXiStarEventCollection.cxx AliXiStar.h AliXiStar.cxx");
    
    // No need for output file names. Procedure is automatic.
    plugin->SetDefaultOutputs(kTRUE);
    //plugin->SetOutputFiles("MyOutput.root");
    // No need define the files to be archived. Note that this is handled automatically by the plugin.
    //plugin->SetOutputArchive("log_archive.zip:stdout,stderr");
    // Set a name for the generated analysis macro (default MyAnalysis.C) Make this unique !
    plugin->SetAnalysisMacro("GeneratedMacro.C");
    // Optionally set maximum number of input files/subjob (default 100, put 0 to ignore). The optimum for an analysis
    // is correlated with the run time - count few hours TTL per job, not minutes !
    plugin->SetSplitMaxInputFileNumber(20); //50
    // Optionally set number of failed jobs that will trigger killing waiting sub-jobs.
    
    // Optionally resubmit threshold.
    plugin->SetMasterResubmitThreshold(97);
    // Optionally set time to live (default 30000 sec)
    plugin->SetTTL(40000);
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
    
    
    Int_t nruns=0,ngoodruns=0;
    if(lhcPeriod=="LHC13c") {
        nruns=14; //14
        Int_t runlist[14]={195677, 195675, 195673, 195644, 195635, 195633, 195596, 195593, 195592, 195568, 195567, 195566, 195531, 195529}; //14
        //195677, 195675, 195673, 195644, 195635, 195633, 195596, 195593, 195592, 195568, 195567, 195566, 195531, 195529
        
        for(Int_t k=0;k<nruns;k++){
            if(MCcase) {
                TString *MCRunName = new TString();
                *MCRunName += runlist[k];
                plugin->AddRunNumber(MCRunName->Data());
            }
            else plugin->AddRunNumber(runlist[k]);
            
            ngoodruns++;
        }
       // plugin->SetNrunsPerMaster(ngoodruns);
       plugin->SetNrunsPerMaster(1);
    }
 
  
    return ngoodruns;
    
}
