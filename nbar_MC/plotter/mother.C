#include "TFile.h"
#include <Riostream.h>
#include "TCanvas.h"
#include "TH1D.h"
#include "TTree.h"
#include "TH2F.h"
#include "TMath.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TLine.h"
#include "TString.h"

static TString file_str = "../../../../vpho_isr_ranked.root";

void ancestor()
{
    gStyle->SetOptStat(0);
    TFile *myf_1 = new TFile(file_str);
    
    TTree *tree = (TTree*)myf_1->Get("tree");
    
    TCanvas *c1 = new TCanvas("c1", "c1",800,600);
    
    tree->Draw("vpho_r_pRecoil>>h1(100,0,7)","","goff");
    tree->Draw("nbar_mcP>>h2(100,0,7)","nbar_mcPDG == -2112 && !TMath::IsNaN(nbar_mcPDG)","goff");
    tree->Draw("nbar_mcP>>h3(100,0,7)","nbar_mcPDG != -2112 && !TMath::IsNaN(nbar_mcPDG)","goff");
    tree->Draw("nbar_varForFirstMCAncestorOfType_anti_n0_p>>h4(100,0,7)"," !TMath::IsNaN(nbar_mcPDG) && nbar_hasAncestor_2112_1 != 0","goff");

    
    TH1 *h1 = (TH1*)gDirectory->Get("h1");
    TH1 *h2 = (TH1*)gDirectory->Get("h2");
    TH1 *h3 = (TH1*)gDirectory->Get("h3");
    TH1 *h4 = (TH1*)gDirectory->Get("h4");
    
    TH1 *hsum = (TH1*)h1->Clone("hSum");
    hsum->Reset();
    hsum->Add(h2);
    hsum->Add(h4);

    cout<<"h1 entries: "<<h1->GetEntries()<<endl;
    cout<<"h2 entries: "<<h2->GetEntries()<<endl;
    cout<<"h3 entries: "<<h3->GetEntries()<<endl;
    cout<<"h4 entries: "<<h4->GetEntries()<<endl;
    cout<<"hsum entries:"<<hsum->GetEntries()<<endl;
    
    delete c1;
    
    h1->SetLineColor(kBlue);
    h2->SetLineColor(kRed);
    h3->SetLineColor(kGreen);
    h4->SetLineColor(kYellow);
    hsum->SetLineColor(kBlack);
    
    
    h1->GetXaxis()->SetTitle("#frac{GeV}{c}");
    h1->GetYaxis()->SetTitle("counts");
    
    
    TString title = "";
    h1->SetTitle(title);
    
    TLegend *leg = new TLegend(0.6,0.6,0.78,0.78);
    leg->AddEntry(h1,"Reconstructed recoil momentum","l");
    leg->AddEntry(h2,"MC momentum of #bar{n} (truth == 1)","l");
    leg->AddEntry(h3,"MC momentum of #bar{n} (truth == 0)","l");
    leg->AddEntry(h4,"Ancestor momentum","l");
    leg->AddEntry(hsum,"Red + Yellow","l");
    
    TCanvas *tela = new TCanvas("tela", "tela");
    
    h1->DrawCopy("HIST");
    h2->DrawCopy("HIST SAMES");
    h3->DrawCopy("HIST SAMES");
    h4->DrawCopy("HIST SAMES");
    hsum->DrawCopy("HIST SAMES");
    leg->Draw("SAME");
    
    TString title_out = "../images/momentum_ancestors.pdf";
    tela->SaveAs(title_out);
    
}


void delta_ancestor()
{
    gStyle->SetOptStat(1110);
    TFile *myf_1 = new TFile(file_str);
    
    TTree *tree = (TTree*)myf_1->Get("tree");
    
    TCanvas *c1 = new TCanvas("c1", "c1",800,600);
    
    tree->Draw("(vpho_r_pRecoil - nbar_mcP)>>h1(100,-1,1)","nbar_mcPDG == -2112","goff");
    tree->Draw("(vpho_r_pRecoil - nbar_varForFirstMCAncestorOfType_anti_n0_p)>>h2(100,-1,1)","!TMath::IsNaN(nbar_mcPDG) && nbar_hasAncestor_2112_1 !=0","goff");
    
    TH1 *h1 = (TH1*)gDirectory->Get("h1");
    TH1 *h2 = (TH1*)gDirectory->Get("h2");
    
    delete c1;
    
    h2->SetLineColor(kBlue);
    //h2->SetLineColor(kRed);
    
    
    h2->GetXaxis()->SetTitle("#Deltap[#frac{GeV}{c}]");
    h2->GetYaxis()->SetTitle("counts");
    
    TString title = "recoil momentum - ancestor momentum";
    h2->SetTitle(title);
    
    TLegend *leg = new TLegend(0.6,0.6,0.78,0.78);
    leg->AddEntry(h1,"pRecoil - #bar{n}p_{MC} (mcPDG == -2112)","l");
    leg->AddEntry(h2,"p recoil - p ancestor","l");
    
    TCanvas *tela = new TCanvas("tela", "tela");
    
    //h1->DrawCopy("HIST");
    h2->DrawCopy("HIST");
    //leg->Draw("SAME");
    
    TString title_out = "../images/deltaP_ancestors.pdf";
    tela->SaveAs(title_out);
    
}

void cluster_ancestor()
{
    gStyle->SetOptStat(0);
    TFile *myf_1 = new TFile(file_str);
    TString var [5] = {"nbar_clusterE","nbar_clusterNHits","nbar_clusterLAT","nbar_clusterSecondMoment","nbar_clusterAbsZernikeMoment51"};
    TString rate [5] = {"(192,0,7)","(128,0,100)","(192,0,1)","(192,0,39)","(192,0,1.2)"};
    
    
    for (int i = 1; i<5; i++)
    {
    
        TTree *tree = (TTree*)myf_1->Get("tree");
        
        TCanvas *c1 = new TCanvas("c1", "c1",800,600);

        
        tree->Draw(var[i] + ">>h1" + rate[i],"nbar_mcPDG == -2112", "goff");
        tree->Draw(var[i] + ">>h2" + rate[i],"TMath::IsNaN(nbar_mcPDG)", "goff");
        tree->Draw(var[i] + ">>h3" + rate[i],"nbar_hasAncestor_2112_1 == 0", "goff");
        tree->Draw(var[i] + ">>h4" + rate[i],"nbar_hasAncestor_2112_1 == 1", "goff");
        tree->Draw(var[i] + ">>h5" + rate[i],"nbar_hasAncestor_2112_1 == 2", "goff");
        
        TH1D *h1 = (TH1D*)gDirectory->Get("h1");
        TH1D *h2 = (TH1D*)gDirectory->Get("h2");
        TH1D *h3 = (TH1D*)gDirectory->Get("h3");
        TH1D *h4 = (TH1D*)gDirectory->Get("h4");
        TH1D *h5 = (TH1D*)gDirectory->Get("h5");
        

        delete c1;
        
        h1->SetLineColor(kBlue);
        h2->SetLineColor(kRed);
        h3->SetLineColor(kGreen);
        h4->SetLineColor(kYellow -6);
        h5->SetLineColor(kBlack);

        
       
        h1->Scale(1.0 / h1->Integral());
        h2->Scale(1.0 / h2->Integral());
        h3->Scale(1.0 / h3->Integral());
        h4->Scale(1.0 / h4->Integral());
        h5->Scale(1.0 / h5->Integral());
        
        
        
        h2->GetXaxis()->SetTitle(var[i]);
        h2->GetYaxis()->SetTitle("counts");
        
        TString title = "";
        h1->SetTitle(title);
        
        TLegend *leg = new TLegend(0.6,0.6,0.78,0.78);
        leg->AddEntry(h1,"nbar_mcPDG = -2112","l");
        leg->AddEntry(h2,"nbar_mcPDG = NaN","l");
        leg->AddEntry(h3,"Ancestor = 0 (no #bar{n} relatives)","l");
        leg->AddEntry(h4,"Ancestor = 1 (#bar{n} as mum)","l");
        //leg->AddEntry(h5,"Ancestor = 2 (#bar{n} as grand-mum)","l");
        
        
        auto tela = std::make_unique<TCanvas>("c1", "c1", 800, 600);
        
        
        h2->Draw("HIST");
        h1->Draw("HIST SAMES");
        h3->Draw("HIST SAMES");
        h4->Draw("HIST SAMES");

        //h5->Draw("HIST SAMES");
        
        leg->Draw("SAME");
        
        TString title_out = "cluster" + var [i] + "_norm.pdf";
        tela->SaveAs(title_out);
        
        delete tree;
        delete h1;
        delete h2;
        delete h3;
        delete h4;
        delete h5;
        delete leg;
        
        
    }
    
}
void vertex_ancestor()
{
    gStyle->SetOptStat(0);
    TFile *myf_1 = new TFile(file_str);
    TString d = "(sqrt(nbar_mcProductionVertexX*nbar_mcProductionVertexX + nbar_mcProductionVertexY*nbar_mcProductionVertexY))";
    
    TTree *tree = (TTree*)myf_1->Get("tree");
    
    TCanvas *c1 = new TCanvas("c1", "c1",800,600);
    
    tree->Draw(d + ">>h1(180,0,220)","","goff");
    tree->Draw(d + ">>h2(180,0,220)","nbar_mcPDG == -2112 && !TMath::IsNaN(nbar_mcPDG)","goff");
    tree->Draw(d + ">>h3(180,0,220)","nbar_hasAncestor_2112_1 == 0 && !TMath::IsNaN(nbar_mcPDG)","goff");
    tree->Draw(d + ">>h4(180,0,220)","nbar_hasAncestor_2112_1 == 1 && !TMath::IsNaN(nbar_mcPDG)","goff");
    
    TH1D *h1 = (TH1D*)gDirectory->Get("h1");
    TH1D *h2 = (TH1D*)gDirectory->Get("h2");
    TH1D *h3 = (TH1D*)gDirectory->Get("h3");
    TH1D *h4 = (TH1D*)gDirectory->Get("h4");
    

    delete c1;
    
    h1->SetLineColor(kBlue);
    h2->SetLineColor(kRed);
    h3->SetLineColor(kGreen);
    h4->SetLineColor(kYellow -6);
    
     
    h1->SetXTitle("#rho = #sqrt{x^2 + y^2} [cm]");
    h1->SetYTitle("counts");
    h1->SetTitle("MC vertex production of #bar{n} candidates");
    
    TLegend *leg = new TLegend(0.6,0.6,0.78,0.78);
    leg->AddEntry(h1,"All MC associated  #bar{n} candidates","l");
    leg->AddEntry(h2,"#bar{n} mcPDG = -2112","l");
    leg->AddEntry(h3,"#bar{n} ancestor = 0","l");
    leg->AddEntry(h4,"#bar{n} ancestor = 1","l");
    
    TCanvas *tela = new TCanvas("tela", "tela");
    
    h1->DrawCopy("HIST");
    h2->DrawCopy("HIST SAMES");
    h3->DrawCopy("HIST SAMES");
    h4->DrawCopy("HIST SAMES");
    leg->Draw("SAME");
    
    TString title_out = "../images/mcDistance_full.pdf";
    tela->SaveAs(title_out);
    
}

void theta_ancestor()
{
    gStyle->SetOptStat(1);
    TFile *myf_1 = new TFile(file_str);
    
    TTree *tree = (TTree*)myf_1->Get("tree");
    
    TCanvas *c1 = new TCanvas("c1", "c1",800,600);
    
    tree->Draw("vpho_r_pRecoilTheta>>h1(100,0,3.14)","","goff");
    tree->Draw("nbar_mcTheta>>h2(100,0,3.14)","!TMath::IsNaN(nbar_mcPDG)","goff");
    tree->Draw("nbar_mcTheta>>h3(100,0,3.14)","nbar_mcPDG == -2112 && !TMath::IsNaN(nbar_mcPDG)","goff");
    tree->Draw("nbar_varForFirstMCAncestorOfType_anti_n0_theta>>h4(100,0,3.14)"," !TMath::IsNaN(nbar_mcPDG)","goff");
    tree->Draw("nbar_theta>>h5(100,0,3.14)","TMath::IsNaN(nbar_mcPDG)","goff");
    
    
    TH1 *h1 = (TH1*)gDirectory->Get("h1");
    TH1 *h2 = (TH1*)gDirectory->Get("h2");
    TH1 *h3 = (TH1*)gDirectory->Get("h3");
    TH1 *h4 = (TH1*)gDirectory->Get("h4");
    TH1 *h5 = (TH1*)gDirectory->Get("h5");
    
    cout<<"h1 entries: "<<h1->GetEntries()<<endl;
    cout<<"h2 entries: "<<h2->GetEntries()<<endl;
    cout<<"h3 entries: "<<h3->GetEntries()<<endl;
    cout<<"h4 entries: "<<h4->GetEntries()<<endl;
    cout<<"h5 entries: "<<h5->GetEntries()<<endl;
    
    delete c1;
    
    h1->SetLineColor(kBlue);
    h2->SetLineColor(kRed);
    h3->SetLineColor(kGreen);
    h4->SetLineColor(kYellow);
    h5->SetLineColor(kBlack);
    
    h1->Scale(1.0 / h1->Integral());
    h2->Scale(1.0 / h2->Integral());
    h3->Scale(1.0 / h3->Integral());
    h4->Scale(1.0 / h4->Integral());
    h5->Scale(1.0 / h5->Integral());
    
    h1->GetXaxis()->SetTitle("rad");
    h1->GetYaxis()->SetTitle("counts");
    h1->SetMaximum(0.03);
    
    TString title = "Normalized #theta distribution ";
    h1->SetTitle(title);
    
    TLegend *leg = new TLegend(0.6,0.6,0.78,0.78);
    leg->AddEntry(h1,"Recoil theta","l");
    leg->AddEntry(h2,"#bar{n} MC_theta","l");
    leg->AddEntry(h3,"#bar{n} MC_theta with mcPDG == -2112","l");
    leg->AddEntry(h4,"Ancestor theta","l");
    leg->AddEntry(h5,"Reconstructed NaN #theta","l");
    
    TCanvas *tela = new TCanvas("tela", "tela");
    
    h1->DrawCopy("HIST");
    h2->DrawCopy("HIST SAMES");
    h3->DrawCopy("HIST SAMES");
    h4->DrawCopy("HIST SAMES");
    h5->DrawCopy("HIST SAMES");
    leg->Draw("SAME");
    
    TString title_out = "theta_ancestors_comparison.pdf";
    tela->SaveAs(title_out);
    
    
}

void has()
{
    gStyle->SetOptStat(0);
    TFile *myf_1 = new TFile(file_str);
    
    TTree *tree = (TTree*)myf_1->Get("tree");
    
    TCanvas *c1 = new TCanvas("c1", "c1",800,600);
    
    tree->Draw("nbar_mcPDG>>h1(100,-50,50)","nbar_hasAncestor_2112_1 ==3","goff");
    
    TH1 *h1 = (TH1*)gDirectory->Get("h1");

    
    cout<<"h1 entries: "<<h1->GetEntries()<<endl;
   
    
    delete c1;
    
    h1->SetLineColor(kBlue);
   
    h1->GetXaxis()->SetTitle("Monte Carlo PDG");
    h1->GetYaxis()->SetTitle("counts");
    
    TString title = "Selection: hasAncestor = 3";
    h1->SetTitle(title);
    
    TCanvas *tela = new TCanvas("tela", "tela");
    
    h1->DrawCopy("HIST");
    
    TString title_out = "../images/mc_pdg_hasAncestor=3.pdf";
    tela->SaveAs(title_out);
}

