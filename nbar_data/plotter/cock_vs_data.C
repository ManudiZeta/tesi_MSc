#include "TFile.h"
#include <Riostream.h>
#include "TCanvas.h"
#include "TH1D.h"
#include "TF1.h"
#include "TTree.h"
#include "TH2F.h"
#include "THStack.h"
#include "TMath.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TLine.h"
#include "TString.h"
#include "TLatex.h"
#include "TGraphErrors.h"

static TString caso = "nog";

static TString datafile = "../../../../200326_nog_data.root";
static TString cockfile = "../../../../180326_nog_qqbar.root";

//static TString datafile = "../../../../260326_isr_data.root";
//static TString cockfile = "../../../../260326_isr_qqbar.root";

void dataMC_nosidebands()
{
    gStyle->SetOptStat(0);
    gStyle->SetEndErrorSize(5);  // Barre errore più visibili [web:27]
    TH1::SetDefaultSumw2();      // Errori propagati automaticamente [web:20]

    TFile *myf_1 = new TFile(datafile);
    TFile *myf_2 = new TFile(cockfile);

    TTree *tree_1 = (TTree*)myf_1->Get("tree");
    TTree *tree_2 = (TTree*)myf_2->Get("tree");

    TString meet_cut = "&& cos(p_theta) > -0.45 && cos(p_theta) < 0.85";
    TString signal   = "(recoil_mass>=0.8 && recoil_mass<=1.2)";
    TString fullcut  = signal + meet_cut;

    TString var[8] = {
        "clusterE", "clusterLAT", "clusterNHits", "clusterSecondMoment",
        "clusterE1E9", "clusterE9E21", "clusterAbsZernikeMoment51", "clusterAbsZernikeMoment40"
    };

    TString lim[8] = {
        "(50,0,3)", "(32,0,1)", "(80,-0.5,79.5)", "(32,0,40)",
        "(32,0,1)", "(32,0,1)", "(32,0,1.2)", "(32,0,1.7)"
    };

    TString um[8] = {" [GeV]", "", "", "", "", "", "", ""};
    TString label[8] = {
        "Cluster Energy", "Cluster LAT", "Cluster N_{hits}", "Cluster Second Moment",
        "Cluster E1E9", "Cluster E9E21", "Zernike Moment 51", "Zernike Moment 51"
    };

    for (int i = 0; i < 8; i++) {

        TString hname_data = Form("hdata_%d", i);
        TString hname_mc   = Form("hmc_%d", i);

        tree_1->Draw("nbar_" + var[i] + ">>" + hname_data + lim[i], fullcut, "goff");
        tree_2->Draw("nbar_" + var[i] + ">>" + hname_mc   + lim[i], fullcut, "goff");

        TH1D *d1 = (TH1D*)gDirectory->Get(hname_data);
        TH1D *c1 = (TH1D*)gDirectory->Get(hname_mc);

        if (!d1 || !c1) {
            std::cout << "Errore istogrammi per " << var[i] << std::endl;
            continue;
        }

        d1->SetDirectory(0);
        c1->SetDirectory(0);
        d1->SetBinErrorOption(TH1::kPoisson);  // Poisson per low stats [web:21]
        c1->SetBinErrorOption(TH1::kPoisson);
        
        // Stile Data (punti + errore Y)
        d1->SetLineColor(kBlue+1);
        d1->SetMarkerColor(kBlue+1);
        d1->SetMarkerStyle(20);
        d1->SetMarkerSize(0.9);
        d1->SetLineWidth(2);

        // Stile MC (filled smooth)
        c1->SetLineColor(kRed+1);
        c1->SetFillColorAlpha(kRed+1, 0.20);
        c1->SetLineWidth(2);

        // Normalizza MC ai Data (errori scalati automaticamente)
        double int_data = d1->Integral();
        double int_mc = c1->Integral();
        if (int_mc > 0) c1->Scale(int_data / int_mc);

        // Canvas con 2 pad
        TCanvas *c = new TCanvas(Form("c_%d", i), Form("c_%d", i), 900, 700);
        TPad *pad1 = new TPad("pad1","pad1", 0.02, 0.30, 0.98, 0.98);
        TPad *pad2 = new TPad("pad2","pad2", 0.02, 0.02, 0.98, 0.28);

        pad1->SetBottomMargin(0.08);
        pad2->SetTopMargin(0);
        pad2->SetBottomMargin(0.15);
        pad1->SetTopMargin(0.03);
        pad2->SetTopMargin(0.08);
        pad1->Draw(); pad2->Draw();

        // PAD SUPERIORE: main plot
        pad1->cd();
        TString title = label[i];
        c1->SetTitle("");
        c1->GetXaxis()->SetTitle(label[i] + um [i]);
        c1->GetYaxis()->SetTitle("counts");
        c1->GetYaxis()->SetTitleOffset(1.25);

        double maxy = 1.35 * std::max(d1->GetMaximum(), c1->GetMaximum());
        c1->SetMinimum(0);
        c1->SetMaximum(maxy);
        c1->Draw("HIST");           // MC filled
        d1->Draw("P EX0 SAME");      // Data: punti + barre Y [web:27]

        TLegend *leg = new TLegend(0.75, 0.8, 0.95, 0.95);
        leg->SetBorderSize(0);
        leg->SetFillStyle(0);
        leg->AddEntry(d1, "Data", "lep");
        leg->AddEntry(c1, "q#bar{q} MC", "lf");
        leg->Draw();

        // PAD INFERIORE: PULL plot
        pad2->cd();
        TH1D *pull = (TH1D*)d1->Clone("pull");
        for(int b=1; b<=d1->GetNbinsX(); b++)
        {

            double val_d = d1->GetBinContent(b);
            double val_c = c1->GetBinContent(b);
            double err_d = d1->GetBinError(b);
            double err_c = c1->GetBinError(b);
            
            if (err_d > 0)
            {
                pull->SetBinContent(b, (val_d - val_c) / sqrt(err_d*err_d + err_c*err_c));
                pull->SetBinError(b, 1.0);
            } else {
                pull->SetBinContent(b, 0);
                pull->SetBinError(b, 0);
            }
        }

        pull->SetTitle("");
        pull->GetXaxis()->SetTitle(label[i] + um [i]);
        pull->GetYaxis()->SetTitle("Pull");
        pull->GetYaxis()->SetTitleSize(0.12);
        pull->GetYaxis()->SetTitleOffset(0.25);
        pull->GetYaxis()->SetLabelSize(0.10);
        pull->GetYaxis()->SetNdivisions(505);
        pull->GetXaxis()->SetLabelSize(0.12);
        pull->SetMaximum(5.01);
        pull->SetMinimum(-5.01);
        pull->SetMarkerStyle(20); pull->SetMarkerSize(0.8);
        pull->SetMarkerColor(kBlack);

        // Linee guida pull
        TLine *l0 = new TLine(pull->GetXaxis()->GetXmin(), 0, pull->GetXaxis()->GetXmax(), 0);
        l0->SetLineWidth(2);
        l0->SetLineStyle(7);
        
        TLine *l1 = new TLine(pull->GetXaxis()->GetXmin(), 1, pull->GetXaxis()->GetXmax(), 1);
        l1->SetLineWidth(1);
        l1->SetLineStyle(9);
        TLine *lm1 = new TLine(pull->GetXaxis()->GetXmin(),-1, pull->GetXaxis()->GetXmax(),-1);
        lm1->SetLineWidth(1);
        lm1->SetLineStyle(9);

        pull->Draw("P EX0");
        l0->Draw();
        l1->Draw();
        lm1->Draw();

        // Copia X axis dal pad1
        c1->GetXaxis()->SetLabelOffset(999);
        c1->GetXaxis()->SetLabelSize(0);
        pull->GetXaxis()->SetTitle(c1->GetXaxis()->GetTitle());
        pull->GetXaxis()->SetTitleSize(0.14);
        pull->GetXaxis()->SetTitleOffset(1.0);

        // Salva
        TString title_out = "../images/dataMC_" + var[i] + "_" + caso + "_pull.pdf";
        c->SaveAs(title_out);

        // Cleanup
        delete l0; delete l1; delete lm1; delete pull; delete pad1; delete pad2; delete c; delete leg; delete d1; delete c1;
    }

    myf_1->Close();
    myf_2->Close();
    delete myf_1;
    delete myf_2;
}

void cluster_comps()
{
    gStyle->SetOptStat(0);
    TFile *myf_1 = new TFile(datafile);
    TFile *myf_2 = new TFile(cockfile);
    
    TTree *tree_1 = (TTree*)myf_1->Get("tree");
    TTree *tree_2 = (TTree*)myf_2->Get("tree");
    
   
    TString meet_cut = "cos(p_theta) > -0.45 && cos(p_theta) < 0.85";
    //TString meet_cut = "";
    
    tree_1->Draw("recoil_mass>>mdata(50,0,2)", meet_cut,"goff");
    tree_2->Draw("recoil_mass>>mcock(50,0,2)", meet_cut,"goff");
    
    
    TH1 *mdata = (TH1*)gDirectory->Get("mdata");
    TH1 *mcock = (TH1*)gDirectory->Get("mcock");
    
    cout<<"Cocktail entries = "<<mcock->GetEntries()<<endl;
    cout<<"Data entries = "<<mdata->GetEntries()<<endl;
    
    mdata->SetLineColor(kBlue);
    mcock->SetLineColor(kRed);
    mcock->Scale(mdata->GetEntries()/mcock->GetEntries());
    
    mcock->GetXaxis()->SetTitle("recoil mass #frac{GeV}{c^{2}}");
    mcock->GetYaxis()->SetTitle("counts");
    mcock->SetTitle("");

    
    TLegend *leg = new TLegend(0.1,0.8,0.38,0.9);
    leg->AddEntry(mdata,"Run1 data","l");
    leg->AddEntry(mcock,"Run1 q#bar{q} cocktail","l");
    
    TCanvas *canv = new TCanvas("tela", "tela");
    
    mcock->DrawCopy("HIST");
    mdata->DrawCopy("HIST SAME");
    leg->Draw("SAME");
    
    TString title_out = "recoil_mass_" + caso + ".pdf";
    canv->SaveAs(title_out);
    
    TString var[8] = {"clusterE","clusterLAT","clusterNHits","clusterSecondMoment","clusterE1E9","clusterE9E21","clusterAbsZernikeMoment51","clusterAbsZernikeMoment40"};
    TString lim [8] = {"(50,0,3)","(64,0,1)","(80,-0.5,79.5)","(64,0,40)","(64,0,1)","(64,0,1)","(64,0,1.2)","(64,0,1.7)"};
    TString um [8] = {" [GeV]","","","","","","",""};
    
    delete leg;
    for(int i = 0; i<8; i++)
    {
        TCanvas *c1 = new TCanvas("c1", "c1",800,600);
        
        tree_1->Draw("nbar_" + var[i] + ">>hdata" + lim[i], meet_cut ,"goff");
        tree_2->Draw("nbar_" + var[i] + ">>hcock" + lim[i], meet_cut,"goff");
        
        
        TH1 *hdata = (TH1*)gDirectory->Get("hdata");
        TH1 *hcock = (TH1*)gDirectory->Get("hcock");
        hcock->Scale(hdata->GetEntries()/hcock->GetEntries());
        
        delete c1;
        
        hdata->SetLineColor(kBlue);
        hcock->SetLineColor(kRed);
        
        hcock->GetXaxis()->SetTitle(var[i] + um[i]);
        hcock->GetYaxis()->SetTitle("counts");
        hcock->SetTitle("Pre sidebands");
        
        double max1 = hdata->GetBinContent(hdata->GetMaximumBin());
        cout<<"max1 = "<<max1<<endl;
        double max2 = hcock->GetBinContent(hcock->GetMaximumBin());
        cout<<"max2 = "<<max2<<endl;
        double max = (max1 > max2) ? max1 : max2;
        //double max = max1;
        cout<<"max = "<<max<<endl;
        
        hdata->SetMaximum(max + 0.1*max);
        hcock->SetMaximum(max + 0.1*max);
        
        TLegend *leg = new TLegend(0.1,0.8,0.38,0.9);
        leg->AddEntry(mdata,"Run1 data","l");
        leg->AddEntry(mcock,"Run1 q#bar{q} cocktail","l");
        
        
        TCanvas *tela = new TCanvas("tela", "tela");
        
        //h1->SetMinimum(0);
        hcock->DrawCopy("HISTO");
        hdata->DrawCopy("HISTO SAME");
        leg->Draw("SAME");
        
        TString title_out = "images/dataVScock_" + var[i] + "_" + caso + "_.pdf";
        tela->SaveAs(title_out);
        
        delete tela;
        delete hdata;
        delete hcock;
        
        
    }
     
    
}

void sidebands()
{
    
    gStyle->SetOptStat(0);
    TFile *myf_1 = new TFile(datafile);
    TFile *myf_2 = new TFile(cockfile);
    
    TTree *tree_1 = (TTree*)myf_1->Get("tree");
    TTree *tree_2 = (TTree*)myf_2->Get("tree");
    
    TString meet_cut = "&& cos(p_theta) > -0.45 && cos(p_theta) < 0.85";
    TString signal = "(recoil_mass>=0.8 && recoil_mass<=1.2)";
    TString sidebands = "((recoil_mass >=0.3 && recoil_mass<=0.7) || (recoil_mass>=1.3 && recoil_mass<=1.7))";
    
    
    TString var[8] = {"clusterE","clusterLAT","clusterNHits","clusterSecondMoment","clusterE1E9","clusterE9E21","clusterAbsZernikeMoment51","clusterAbsZernikeMoment40"};
    TString lim [8] = {"(50,0,3)","(32,0,1)","(40,0,80)","(32,0,40)","(32,0,1)","(32,0,1)","(32,0,1.2)","(32,0,1.7)"};
    TString um [8] = {" [GeV]","","","","","","",""};

    
    for(int i = 0; i<8; i++)
    {
        TCanvas *canv1 = new TCanvas("c1", "c1",800,600);
        
        tree_1->Draw("nbar_" + var[i] + ">>d1" + lim[i],signal + meet_cut ,"goff");
        tree_1->Draw("nbar_" + var[i] + ">>d2" + lim[i],sidebands + meet_cut,"goff");
        
        tree_2->Draw("nbar_" + var[i] + ">>c1" + lim[i],signal + meet_cut ,"goff");
        tree_2->Draw("nbar_" + var[i] + ">>c2" + lim[i],sidebands + meet_cut,"goff");
        
        
        TH1 *d1 = (TH1*)gDirectory->Get("d1");
        TH1 *d2 = (TH1*)gDirectory->Get("d2");
        d2->Scale(0.5);
        
        TH1 *c1 = (TH1*)gDirectory->Get("c1");
        TH1 *c2 = (TH1*)gDirectory->Get("c2");
        d2->Scale(0.5);
        
        delete canv1;
        
        d1->Add(d2, -1.0);
        d1->SetLineColor(kBlue);
        //d2->SetLineColor(kRed);
        //f->SetLineColor(kRed);
        
        c1->Add(c2, -1.0);
        c1->SetLineColor(kRed);
        //c2->SetLineColor(kRed);
        //f->SetLineColor(kRed);
        
        c1->Scale(d1->GetEntries()/c1->GetEntries());
        
        
        c1->GetXaxis()->SetTitle(var[i] + um[i]);
        c1->GetYaxis()->SetTitle("counts");
        
        
        TString title = "After sidebands";
        c1->SetTitle(title);
        
        TLegend *leg = new TLegend(0.1,0.8,0.38,0.9);
        leg->AddEntry(d1,"Run1 data","l");
        leg->AddEntry(c1,"Run1 q#bar{q} cocktail","l");
        
        
        TCanvas *tela = new TCanvas("tela", "tela");
        
        c1->SetMinimum(0);
        c1->DrawCopy("HISTO");
        d1->DrawCopy("HISTO SAME");
        //f->Draw("SAME");
        leg->Draw("SAME");
        
        TString title_out = "images/sidebands_dataVScock_" + var[i] + "_" + caso +  "_.pdf";
        tela->SaveAs(title_out);
        
        delete tela;
        delete d1;
        delete d2;
        
        delete c1;
        delete c2;
        
        
    }
    
}

void recoil_mass ()
{
    //gStyle->SetOptStat(0);
    gStyle->SetOptStat(0);

    TFile *myf_1 = new TFile(datafile);
    TTree *tree_1 = (TTree*)myf_1->Get("tree");
    TString extra_cut = "cos(p_theta) > -0.45 && cos(p_theta) < 0.85 ";
    
    TCanvas *c1 = new TCanvas("c1", "c1",800,600);
    tree_1->Draw("recoil_mass>>histo1(100,0,2)",extra_cut);
    
       
    
    delete c1;
    
    TH1D *histo1 = (TH1D*)gDirectory->Get("histo1");

    histo1->SetLineColor(kBlue);
    
    TString title_x = "recoil mass [#frac{GeV}{c^{2}}]";
    histo1->GetXaxis()->SetTitle(title_x);
    histo1->GetYaxis()->SetTitle("counts");
    histo1->SetTitle("");
    
    TCanvas *tela = new TCanvas("tela", "tela");
    
    histo1->DrawCopy("HIST");
    
    TString title_out = "../images/recoil_mass_data_postcut.pdf";
    tela->SaveAs(title_out);
    
}

void protons_distro ()
{
    //gStyle->SetOptStat(0);
    gStyle->SetOptStat(0);

    TFile *myf_1 = new TFile(datafile);
    TTree *tree_1 = (TTree*)myf_1->Get("tree");
    TString extra_cut = "";
    
    TCanvas *c1 = new TCanvas("c1", "c1",800,600);
    tree_1->Draw("p_p:cos(p_theta)>>histo1(300,-1,+1,300,0,7)","");
    
       
    
    delete c1;
    
    TH2 *histo1 = (TH2*)gDirectory->Get("histo1");

    histo1->SetLineColor(kBlue);
    
    histo1->GetXaxis()->SetTitle("cos(#theta)");
    histo1->GetYaxis()->SetTitle("p [#frac{GeV}{c}]");
    histo1->SetTitle("");
    
    TCanvas *tela = new TCanvas("tela", "tela");
    
    histo1->DrawCopy("HIST");
    
    TString title_out = "../images/proton_costheta_p.pdf";
    tela->SaveAs(title_out);
    
}

void pure_cluster()
{
    gStyle->SetOptStat(0);
    TFile *myf_1 = new TFile(datafile);
    TTree *tree_1 = (TTree*)myf_1->Get("tree");
    
    TString meet_cut = "cos(p_theta) > -0.45 && cos(p_theta) < 0.85";
            
    TString var[8] = {"clusterE","clusterLAT","clusterNHits","clusterSecondMoment","clusterE1E9","clusterE9E21","clusterAbsZernikeMoment51","clusterAbsZernikeMoment40"};
    TString lim [8] = {"(50,0,3)","(64,0,1)","(80,-0.5,79.5)","(64,0,40)","(64,0,1)","(64,0,1)","(64,0,1.2)","(64,0,1.7)"};
    TString um [8] = {" [GeV]","","","","","","",""};
    
    for(int i = 0; i<8; i++)
    {
        TCanvas *c1 = new TCanvas("c1", "c1",800,600);
        
        tree_1->Draw("nbar_" + var[i] + ">>hdata" + lim[i], meet_cut ,"goff");
    
        TH1 *hdata = (TH1*)gDirectory->Get("hdata");
        
        delete c1;
    
        hdata->SetLineColor(kBlue);
    
        hdata->GetXaxis()->SetTitle(var[i] + um[i]);
        hdata->GetYaxis()->SetTitle("counts");
        hdata->SetTitle("");
     
        TCanvas *tela = new TCanvas("tela", "tela");
        
        hdata->DrawCopy("HISTO");
        
        TString title_out = "../images/data_" + var[i] + "_" + caso + ".pdf";
        tela->SaveAs(title_out);
        
        delete tela;
        delete hdata;
    
    }
     
    
}

