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
#include "TColor.h"
#include "TLatex.h"
#include "TGraphErrors.h"

TString caso = "nog";



/*
static TString file_uu = "../../../../qqbar/isr/260326_isr_uubar_tot.root";
static TString file_dd = "../../../../qqbar/isr/260326_isr_ddbar_tot.root";
static TString file_cc = "../../../../qqbar/isr/260326_isr_ssbar_tot.root";
static TString file_ss = "../../../../qqbar/isr/260326_isr_ccbar_tot.root";
*/

static TString file[4] ={
    "../../../../qqbar/nog/180326_nog_uubar_tot.root",
    "../../../../qqbar/nog/180326_nog_ddbar_tot.root",
    "../../../../qqbar/nog/180326_nog_ssbar_tot.root",
    "../../../../qqbar/nog/180326_nog_ccbar_tot.root"
};

static TString nome[4] ={"u#bar{u}","d#bar{d}","s#bar{s}","c#bar{c}"};

void single_contr()
{
    gStyle->SetOptStat(0);

    TFile *myf[4];
    TTree *tree[4];
    TTree *topo[4];
    TH1D *histo[4];
    THStack *hs = new THStack("hs","");
    TLegend *leg = new TLegend(0.6,0.6,0.78,0.78);

    for (int i = 3; i>=0; i--)
    {
        TString name = Form("mtot_%d", i);
        myf[i] = new TFile(file[i]);
        tree[i] = (TTree*)myf[i]->Get("tree");
        histo[i] = new TH1D(name, name, 100, 0, 2);
        tree[i]->Draw("recoil_mass>>" + name + "(100,0.,2.)", "", "goff");
        histo[i] = (TH1D*)gDirectory->Get(name);
        histo[i]->SetLineColor(i+6);
        histo[i]->SetFillColor(i+6);
        hs->Add(histo[i]);
        leg->AddEntry(histo[i],nome[i],"l");
   }
    TCanvas *tela = new TCanvas("tela", "tela");
    hs->Draw("HISTO");
    leg->Draw("SAME");
    hs->GetXaxis()->SetTitle("recoil mass [#frac{GeV}{c^{2}}]");
    hs->GetYaxis()->SetTitle("counts");
    gPad->Update();
    TString title_out = "../images/single_contr_of_qqbar_nog.pdf";
    tela->SaveAs(title_out);
}

