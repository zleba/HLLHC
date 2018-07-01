/*
 * ntupleQCD.cpp
 *
 *  Created on: 24 Aug 2016
 *      Author: jkiesele
 */

#include "interface/ntupleQCD.h"

#include <iostream>

//#include "Math/PtEtaPhiM4D.h"
//typedef ROOT::Math::PtEtaPhiM4D<float> FourVector;

#include "TString.h"
#define SF TString::Format

using namespace std;

struct Jets {

    Jets(bool _isRec, bool _withStructure) : isRec(_isRec), withStructure(_withStructure) {}

    std::vector<float> pt_, eta_, phi_, mass_,
        tau1_, tau2_, tau3_,
        ptSD_[2], etaSD_[2], phiSD_[2], massSD_[2],
        jetflavour_, btag_; 
    //std::vector<TLorentzVector> p4;
    bool isRec, withStructure;

    void Reset()
    {
        pt_.clear();
        eta_.clear();
        phi_.clear();
        mass_.clear();
        //p4.clear();

        tau1_.clear();
        tau2_.clear();
        tau3_.clear();
        for(int i = 0; i < 2; ++i) {
            ptSD_[i].clear();
            etaSD_[i].clear();
            phiSD_[i].clear();
            massSD_[i].clear();
        }
        jetflavour_.clear();
        btag_.clear();
    }


    void InitBranches(TTree* myskim, string nameTag)
    {
        auto initOne =[&](string name, std::vector<float> *var) {
            myskim->Branch((nameTag+name).c_str(), "vector<float>",  var);
        };

        initOne("jetPt"           , &pt_);
        initOne("jetEta"          , &eta_);
        initOne("jetPhi"          , &phi_);
        initOne("jetMass"         , &mass_);

        //myskim->Branch((nameTag+"p4").c_str(), &p4);

        if(withStructure) {
            initOne("jetTau1"         , &tau1_);
            initOne("jetTau2"         , &tau2_);
            initOne("jetTau3"         , &tau3_);

            for(int i = 0; i < 2; ++i) {
                initOne(SF("jetSD%dPt"  ,i+1).Data() , &ptSD_[i]);
                initOne(SF("jetSD%dEta" ,i+1).Data() , &etaSD_[i]);
                initOne(SF("jetSD%dPhi" ,i+1).Data() , &phiSD_[i]);
                initOne(SF("jetSD%dMass",i+1).Data() , &massSD_[i]);
            }
        }

        initOne("jetFlavour"      , &jetflavour_);
        if(isRec)
            initOne("jetBtag"         , &btag_);  
    }

    void PushValues(d_ana::dBranchHandler<Jet> &jets)
    {
        for(size_t i=0;i<jets.size();i++){
            pt_.push_back(jets.at(i)->PT);
            eta_.push_back(jets.at(i)->Eta); 
            phi_.push_back(jets.at(i)->Phi); 
            mass_.push_back(jets.at(i)->Mass); 

            //p4.push_back(TLorentzVector(jets.at(i)->PT, jets.at(i)->Eta, jets.at(i)->Phi,jets.at(i)->Mass));

            if(withStructure) {
                tau1_.push_back(jets.at(i)->Tau[0]);
                tau2_.push_back(jets.at(i)->Tau[1]);
                tau3_.push_back(jets.at(i)->Tau[2]);

                for(int k = 0; k < 2; ++k) {
                    double Pt=0, Eta=0, Phi=0, M=0; //Dummy values
                    if(k < jets.at(i)->NSubJetsSoftDropped) {
                        Pt  = jets.at(i)->SoftDroppedP4[k].Pt();
                        Eta = jets.at(i)->SoftDroppedP4[k].Eta();
                        Phi = jets.at(i)->SoftDroppedP4[k].Phi();
                        M   = jets.at(i)->SoftDroppedP4[k].M();
                    }
                    ptSD_[k].push_back(Pt);
                    etaSD_[k].push_back(Eta);
                    phiSD_[k].push_back(Phi);
                    massSD_[k].push_back(M);
                }

                //for(int k = 0; k < jets.at(i)->NSubJetsSoftDropped; ++k)
                    //cout <<"Jets " << i <<" "<< k << " : "<<  jets.at(i)->SoftDroppedP4[k].Pt() << endl;
            }

            jetflavour_.push_back(jets.at(i)->Flavor);
            if(isRec)
                btag_.push_back(jets.at(i)->BTag);
        }
    }

};



void ntupleQCD::analyze(size_t childid /* this info can be used for printouts */){

	/*
	 * This skeleton analyser runs directly on the Delphes output.
	 * It can be used to create histograms directly or a skim.
	 * If a skim is created, a new input configuration will be written automatically
	 * and stored in the output directory together with the ntuples.
	 * The skim can contain delphes objects again or can be flat. This is up
	 * to the user.
	 * Examples for both are given here.
	 *
	 * The same skeleton can be used to read the skim. Please refer to the comments
	 * marked with "==SKIM=="
	 *
	 * These parts are commented, since the code is supposed to work as an example without
	 * modifications on Delphes output directly.
	 */



	/*
	 * Define the branches that are to be considered for the analysis
	 * This branch handler (notice the "d")
	 * is used to run directly in Delphes output.
	 * For skimmed ntuples, see below
	 */

	//d_ana::dBranchHandler<Electron> elecs(tree(),"Electron");

	/*
	 * Other branches might be the following
	 * (for a full list, please inspect the Delphes sample root file with root)
	 * For the Delphes class description, see $DELPHES_PATH/classes/DelphesClasses.h
	 */
	//
	d_ana::dBranchHandler<HepMCEvent>  event(tree(),"Event");
	//d_ana::dBranchHandler<GenParticle> genpart(tree(),"Particle");
	d_ana::dBranchHandler<Jet>         genjet(tree(),"GenJet");
	d_ana::dBranchHandler<Jet>         jet(tree(),"Jet");

	d_ana::dBranchHandler<Jet>         genjetAK8(tree(),"GenJetAK8");
	d_ana::dBranchHandler<Jet>         jetAK8(tree(),"JetAK8");

	//d_ana::dBranchHandler<Muon>        muontight(tree(),"MuonTight");
	//d_ana::dBranchHandler<Muon>        muonloose(tree(),"MuonLoose");
	//d_ana::dBranchHandler<Photon>      photon(tree(),"Photon");
	d_ana::dBranchHandler<MissingET>   met(tree(),"MissingET");
	d_ana::dBranchHandler<ScalarHT>    scalarHT(tree(),"ScalarHT");
	d_ana::dBranchHandler<Vertex>      vertex(tree(),"Vertex");


	/* ==SKIM==
	 *
	 * If a skim of the Delphes outout was created in a way indicated
	 * further below, use the tBranchHandler (please notive the "t")
	 * to access vectors of objects...
	 *
	 */
	// d_ana::tBranchHandler<std::vector<Electron> > electrons(tree(),"Electrons");

	/*==SKIM==
	 *
	 * Or an object directly
	 *
	 */
	//d_ana::tBranchHandler<MissingET> met(tree(),"MET");



	/*
	 * Always use this function to add a new histogram (can also be 2D)!
	 * Histograms created this way are automatically added to the output file
	 */
	//TH1* histo=addPlot(new TH1D("histoname1","histotitle1",100,0,100),"p_{T} [GeV]","N_{e}"); //commented PG


	/*
	 * If (optionally) a skim or a flat ntuple is to be created, please use the following function to initialize
	 * the tree.
	 * The output files will be written automatically, and a config file will be created.
	 */
	TTree* myskim=addTree();
	/*
	 * Add a simple branch to the skim
	 */
    /*
    */

    float weight, scale;
    int processID;

    myskim->Branch("weight",      &weight   ,  "weight/F"   );
    myskim->Branch("scale",       &scale    ,  "scale/F"    );
    myskim->Branch("processID",   &processID,  "processID/I");

    float metEt, metPhi, sumHT;
    myskim->Branch("metEt",   &metEt,  "metEt/F");
    myskim->Branch("metPhi",  &metPhi, "metPhi/F");
    myskim->Branch("sumHT",   &sumHT,  "sumHT/F");

    int nVtx;
    myskim->Branch("nVtx",   &nVtx,   "nVtx/I");


	//jets

    Jets recJets(true, false),    genJets(false, false);
    Jets recJetsAK8(true, true), genJetsAK8(false, true);

    recJets.InitBranches(myskim, "");
    genJets.InitBranches(myskim, "gen");

    recJetsAK8.InitBranches(myskim, "AK8");
    genJetsAK8.InitBranches(myskim, "AK8gen");


	size_t nevents=tree()->entries();
	if(isTestMode())
		nevents/=100;
	for(size_t eventno=0;eventno<nevents;eventno++) {
	  /*
	   * The following two lines report the status and set the event link
	   * Do not remove!
	   */
	  reportStatus(eventno,nevents);
	  tree()->setEntry(eventno);

      recJets.PushValues(jet);
      genJets.PushValues(genjet);
      recJetsAK8.PushValues(jetAK8);
      genJetsAK8.PushValues(genjetAK8);

      weight = event.at(0)->Weight;
      scale = event.at(0)->Scale;
      processID = event.at(0)->ProcessID;
      //cout << scale <<" "<< weight<<" "<< processID<< endl;

      metEt  = met.at(0)->MET;
      metPhi = met.at(0)->Phi;
      sumHT  = scalarHT.at(0)->HT;

      nVtx = vertex.size();
      //cout << nVtx << endl;
      //cout << metEt <<" "<< metPhi << " "<< sumHT  << endl;


	  myskim->Fill();
      recJets.Reset();
      genJets.Reset();
      recJetsAK8.Reset();
      genJetsAK8.Reset();

	}

	
	/*
	 * Must be called in the end, takes care of thread-safe writeout and
	 * call-back to the parent process
	 */
	processEndFunction();
}



void ntupleQCD::postProcess(){
	/*
	 * This function can be used to analyse the output histograms, e.g. extract a signal contribution etc.
	 * The function can also be called directly on an output file with the histograms, if
	 * RunOnOutputOnly = true
	 * is set in the analyser's config file
	 *
	 * This function also represents an example of how the output of the analyser can be
	 * read-back in an external program.
	 * Just include the sampleCollection.h header and follow the procedure below
	 *
	 */

	/*
	 * Here, the input file to the extraction of parameters from the histograms is the output file
	 * of the parallelised analysis.
	 * The sampleCollection class can also be used externally for accessing the output consistently
	 */
  //d_ana::sampleCollection samplecoll;
  //	samplecoll.readFromFile(getOutPath());
  
  //	std::vector<TString> alllegends = samplecoll.listAllLegends();
  
  /*
   * Example how to process the output.
   * Usually, one would define the legendname of the histogram to be used here
	 * by hand, e.g. "signal" or "background".
	 * To make this example run in any case, I am using alllegends.at(0), which
	 * could e.g. be the signal legend.
	 *
	 * So in practise, the following would more look like
	 * samplecoll.getHistos("signal");
	 */
  //if(alllegends.size()>0){
  //d_ana::histoCollection histos=samplecoll.getHistos(alllegends.at(0));

		/*
		 * here, the histogram created in the analyze() function is selected and evaluated
		 * The histoCollection maintains ownership (you don't need to delete the histogram)
		 */
		//const TH1* myplot=histos.getHisto("histoname1"); //commented by PG

		//std::cout << "(example output): the integral is " << myplot->Integral() <<std::endl;

		/*
		 * If the histogram is subject to changes, please clone it and take ownership
		 */

		//TH1* myplot2=histos.cloneHisto("histoname1"); //commented by PG

		/*
		 * do something with the histogram
		 */

		//delete myplot2;
  //	}

	/*
	 * do the extraction here.
	 */



}



