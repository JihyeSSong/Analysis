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
    plugin->SetRunMode("test");  // VERY IMPORTANT
    // Set versions of used packages
    plugin->SetAPIVersion("V1.1x");
    //    plugin->SetROOTVersion("v5-34-05");
    //    plugin->SetAliROOTVersion("v5-04-80-AN");
    
    plugin->SetROOTVersion("v5-34-08-6");
    plugin->SetAliROOTVersion("vAN-20141030");
    
    
    if(!MCcase) plugin->SetRunPrefix("000");   // real or MC data
    
    
    TString *Production=new TString("11a");
    
    Int_t totruns=0;
    // Declare input data to be processed.
    if(!MCcase){// Real data
        
   
        if(Production->Contains("11a")){// ESDs
            plugin->SetGridDataDir("/alice/data/2011/LHC11a");
            plugin->SetDataPattern("ESDs/pass4_without_SDD/*AliESDs.root"); // following Victors analysis note period (rho analysis)
            totruns += AddGoodRuns(plugin,"LHC11a");
        }
       
        /*
         if(Production->Contains("11a")){// ESDs
         plugin->SetGridDataDir("/alice/data/2011/LHC11a");
         plugin->SetDataPattern("ESDs/pass4_with_SDD/*AliESDs.root");
         totruns += AddGoodRuns(plugin,"LHC11a");
         }
 */
         if(Production->Contains("10d")){// AODs
         plugin->SetGridDataDir("/alice/data/2010/LHC10d");
         plugin->SetDataPattern("ESDs/pass2/*AliESDs.root");
         totruns += AddGoodRuns(plugin,"LHC10d");
         }
        
        if(Production->Contains("13c")){// ESDs
            plugin->SetGridDataDir("/alice/data/2013/LHC13c");
            plugin->SetDataPattern("pass2/*AliESDs.root");
            totruns += AddGoodRuns(plugin,"LHC13c");
        }
        
     
        if(Production->Contains("10e")){// ESDs
            plugin->SetGridDataDir("/alice/data/2010/LHC10e");
            plugin->SetDataPattern("ESDs/pass2/*AliESDs.root");
            totruns += AddGoodRuns(plugin,"LHC10e");
        }
    }else {// MC data
        if(Production->Contains("13c")){// AODs
            plugin->SetGridDataDir("/alice/sim/2013/LHC13b2_efix_p1");
            plugin->SetDataPattern("*AliESDs.root");
            totruns += AddGoodRuns(plugin,"LHC13c",MCcase);
        }
        
        if(Production->Contains("10h")){// ESDs
            plugin->SetGridDataDir("/alice/sim/LHC11a10a_bis");
            plugin->SetDataPattern("*AliESDs.root");
            totruns += AddGoodRuns(plugin,"LHC10h",MCcase);
        }
        
             if(Production->Contains("10d")){// ESDs
            plugin->SetGridDataDir("/alice/sim/LHC10d4");
            plugin->SetDataPattern("*AliESDs.root");
            totruns += AddGoodRuns(plugin,"LHC10d",MCcase);
        }
    }
    
    
    
    
    // Define alien work directory where all files will be copied. Relative to alien $HOME.
    if(!MCcase) plugin->SetGridWorkingDir("pp276TeV_LHC11a_withoutSDD");
  //  else plugin->SetGridWorkingDir("lhc13b2_efix_p2_Cent_Nov29");
  //  else plugin->SetGridWorkingDir("lhc13b2_efix_p2_Cent_Nov30");
    else plugin->SetGridWorkingDir("LHC10d_MC_LHC10d4_march27"); // before merging

    
    // Declare alien output directory. Relative to working directory.
    plugin->SetGridOutputDir("output"); // In this case will be $HOME/work/output
    //plugin->SetMaxMergeFiles(50);
    plugin->SetOneStageMerging(kFALSE);
    plugin->SetMaxMergeStages(2);
    plugin->SetMergeViaJDL();
    //plugin->SetUseSubmitPolicy();
    
    // Declare the analysis source files names separated by blancs. To be compiled runtime
    // using ACLiC on the worker nodes.
    plugin->SetAnalysisSource("AliXiStarppEventCollection.cxx AliXiStarpp.cxx");
    // Declare all libraries (other than the default ones for the framework. These will be
    // loaded by the generated analysis macro. Add all extra files (task .cxx/.h) here.
    plugin->SetAdditionalLibs("AliXiStarppEventCollection.h AliXiStarppEventCollection.cxx AliXiStarpp.h AliXiStarpp.cxx");
    
    // No need for output file names. Procedure is automatic.
    plugin->SetDefaultOutputs(kTRUE);
    //plugin->SetOutputFiles("MyOutput.root");
    // No need define the files to be archived. Note that this is handled automatically by the plugin.
    //plugin->SetOutputArchive("log_archive.zip:stdout,stderr");
    // Set a name for the generated analysis macro (default MyAnalysis.C) Make this unique !
    plugin->SetAnalysisMacro("GeneratedMacro.C");
    // Optionally set maximum number of input files/subjob (default 100, put 0 to ignore). The optimum for an analysis
    // is correlated with the run time - count few hours TTL per job, not minutes !
    plugin->SetSplitMaxInputFileNumber(50);
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
    
    
    if(lhcPeriod=="LHC11a") {
        nruns=24;
        Int_t runlist[24]={146686, 146688, 146689, 146746, 146747, 146748, 146801, 146802, 146803, 146804, 146805, 146806, 146807, 146808, 146812, 146813, 146814, 146817, 146824, 146856, 146857, 146858, 146859, 146860
};
        
        
      //  {122374, 122375, 124183, 124186, 124187, 124355, 124358, 124359, 124360, 124362, 124367, 124371, 124374, 124378, 124380, 124381, 124383, 124385, 124388, 124600, 124603, 124604, 124605, 124606, 124607, 124608, 124751, 125023, 125083, 125085, 125097, 125100, 125101, 125131, 125133, 125134, 125139, 125140, 125156, 125186, 125292, 125295, 125296, 125628, 125630, 125632, 125633, 125842, 125843, 125844, 125847, 125848, 125849, 125850, 125851, 125855, 126004, 126007, 126008, 126073, 126078, 126081, 126082, 126086, 126087, 126088, 126090, 126097, 126158, 126160, 126162, 126167, 126168, 126177, 126283, 126284, 126285, 126350, 126351, 126352, 126359, 126403, 126404, 126405, 126406, 126407, 126408, 126409, 126422, 126424, 126425, 126432, 126437}; // lhc10b Data 93 runs
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
