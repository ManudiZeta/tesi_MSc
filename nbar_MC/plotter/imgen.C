#include "TFile.h"
#include <Riostream.h>
#include "TCanvas.h"
#include "TH1D.h"
#include "TTree.h"
#include "TH2F.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TString.h"
#include "TLine.h"
#include "TPaveText.h"

void una_grand (TString file_1 = "../../../../vpho_isr_ranked.root" , TString file_2 = "../../../../vpho_nog_ranked.root")
{
    //gStyle->SetOptStat(0);
    gStyle->SetOptStat(1110);
    
    TFile *myf_1 = nullptr;
    TFile *myf_2 = nullptr;
    
    ifstream in_f1(file_1);
    ifstream in_f2(file_2);
    
    if(!in_f1 | !in_f2)
    {
        cout<<"Selected root file doesn't exist. Please, generate it. \n";
        return;
    }
    in_f1.close();
    in_f2.close();
    
    myf_1 = new TFile(file_1);
    myf_2 = new TFile(file_2);
    
    TTree *tree_1 = (TTree*)myf_1->Get("tree");
    TTree *tree_2 = (TTree*)myf_2->Get("tree");
    
    TCanvas *c1 = new TCanvas("c1", "c1",800,600);

    TString var = "vpho_r_pRecoil-nbar_mcP";
    TString drawExpr1 = var + ">>histo1(100,-0.2,0.2)";
    TString drawExpr2 = var + ">>histo2(100,-0.2,0.2)";
    tree_1->Draw(drawExpr1,"nbar_mcPDG == -2112");
    tree_2->Draw(drawExpr2,"nbar_mcPDG == -2112");
       
    
    delete c1;

    
    TH1D *histo1 = (TH1D*)gDirectory->Get("histo1");
    TH1D *histo2 = (TH1D*)gDirectory->Get("histo2");
    
    histo1->SetLineColor(kBlue);
    histo2->SetLineColor(kRed);
    
    double max1 = histo1->GetBinContent(histo1->GetMaximumBin());
    cout<<"max1 = "<<max1<<endl;
    double max2 = histo2->GetBinContent(histo2->GetMaximumBin());
    cout<<"max2 = "<<max2<<endl;
    double max = (max1 > max2) ? max1 : max2;
    cout<<"max = "<<max<<endl;
    
    histo1->SetMaximum(max + 0.1*max);
    histo2->SetMaximum(max + 0.1*max);
    
    TString title_x = "#Delta p [#frac{GeV}{c}]";
    histo1->GetXaxis()->SetTitle(title_x);
    histo1->GetYaxis()->SetTitle("counts");
    TString title = "p recoil - p #bar{n} MC ";
    histo1->SetTitle(title);
    
    TLegend *leg = new TLegend(0.6,0.6,0.78,0.78);
    leg->AddEntry(histo1,"ISR","l");
    leg->AddEntry(histo2,"NO ISR","l");
    
    TCanvas *tela = new TCanvas("tela", "tela");
    
    histo1->DrawCopy("HIST");
    histo2->DrawCopy("HIST SAMES");
    leg->Draw("SAME");
    
    TString title_out = "../images/diff_pREC_np_mc.pdf";
    tela->SaveAs(title_out);
    
}


void due_grand(TString var, TString cut, TString um, TString x1, TString x2)
{
    gStyle->SetOptStat(1);
    string in_1, in_2;
    TFile *myf_1 = nullptr;
    
    
    in_1 = "../../../tot_nog_06012026.root";
    
    ifstream in_f1(in_1);
    
    if(!in_f1)
    {
        cout<<"Selected root file doesn't exist. Please, generate it. \n";
        return;
    }
    in_f1.close();
    
    myf_1 = new TFile("../../../tot_nog_06012026.root");
    
    TTree *tree_1 = (TTree*)myf_1->Get("tree");
    TTree *tree_2 = (TTree*)myf_1->Get("tree");
    
    TCanvas *c1 = new TCanvas("c1", "c1",800,600);
    TString var_1 = "nbar_" + var;
    TString var_2 = "nbar_" + var;
    cout<<"var_1 = "<<var_1<<endl;
    cout<<"var_2 = "<<var_2<<endl;
    
    TString drawExpr1 = var_1 + ">>histo1(256," + x1 + "," + x2 + ")";
    TString drawExpr2 = var_2 + ">>histo2(256," + x1 + "," + x2 + ")";
    TString cut_2 = "nbar_" + cut;
    cout<<"cut_2 = "<<cut_2<<endl;
    tree_1->Draw(drawExpr1, "", "goff");
    tree_2->Draw(drawExpr2, cut_2, "goff");
    
    
    delete c1;
    TH1 *histo1 = (TH1D*)gDirectory->Get("histo1");
    TH1 *histo2 = (TH1D*)gDirectory->Get("histo2");
    
    histo1->SetLineColor(kBlue);
    histo2->SetLineColor(kRed);
    histo1->SetFillColor(kBlue);
    histo2->SetFillColor(kRed);
    
    TString title_x = var + " [" + um + "]";
    //TString title_x = "clusterSecondMoment []";
    histo1->GetXaxis()->SetTitle(title_x);
    histo1->GetYaxis()->SetTitle("counts []");
    
    
    TString title = "";
    histo1->SetTitle(title);
    
    
    cout<<"NEntries_1 = "<<histo1->GetEntries()<<endl;
    cout<<"NEntries_2 = "<<histo2->GetEntries()<<endl;
    
    TLegend *leg = new TLegend(0.6,0.6,0.78,0.78);
    leg->AddEntry(histo1,"all #bar{n} candidates ","l");
    leg->AddEntry(histo2,cut_2,"l");
    
    TCanvas *tela = new TCanvas("tela", "tela");
    
    histo1->DrawCopy("HIST");
    histo2->DrawCopy("HIST SAMES");
    leg->Draw("SAME");
    
    TString title_out = "../images/continuum/" + var +"_-2112.pdf";
    tela->SaveAs(title_out);
    
}



void corr (TString file_1 = "../../../../vpho_isr_ranked.root")
{
    gStyle->SetOptStat(0);

    TFile *myf_1 = new TFile(file_1);
    
    TTree *tree_1 = (TTree*)myf_1->Get("tree");
    
    TCanvas *c1 = new TCanvas("c1", "c1",800,600);
    TString var = "vpho_r_pRecoil:nbar_p";
    TString drawExpr1 = var + ">>histo1(100,0,7,100,0,7)";
    tree_1->Draw(drawExpr1,"nbar_mcPDG == -2112","colz");
    
    
    delete c1;
    
    TH2 *histo1 = (TH2D*)gDirectory->Get("histo1");
    double rho = histo1->GetCorrelationFactor();
    histo1->SetLineColor(kBlue);
    cout << "Correlation factor = " << rho << endl;
    
    histo1->GetXaxis()->SetTitle("#bar{n} reconstructed p [#frac{GeV}{c}]");
    histo1->GetYaxis()->SetTitle("recoil p [#frac{GeV}{c}]");
    histo1->SetTitle("ISR");
    
    TLine *tl = new TLine(0,0,7,7);
    
    TCanvas *tela = new TCanvas("tela", "tela");
    
    TPaveText *pt = new TPaveText(0.15, 0.80, 0.45, 0.90, "NDC");
    pt->SetFillColor(0);
    pt->SetBorderSize(1);
    pt->SetTextAlign(12);
    pt->SetTextSize(0.035);
    pt->AddText(Form("#rho = %.3f", rho));
    
    
    histo1->DrawCopy("HIST");
    tl->Draw("same");
    pt->Draw("same");
    //TString title_out = "../images/gen_" + var + ".pdf";
    TString title_out = "../images/gen_rec_p_corr_isr.pdf";
    tela->SaveAs(title_out);
    
    
}



void kin()
{
    gStyle->SetOptStat(1110);
    TString in_1, in_2;
    TFile *myf_1 = nullptr;
    TFile *myf_2 = nullptr;
    
    in_1 = "../../../../vpho_isr_ranked_kin.root";
    in_2 = "../../../../vpho_nog_ranked_kin.root";
    
    ifstream in_f1(in_1);
    ifstream in_f2(in_2);
    
    if(!in_f1 | !in_f2)
    {
        cout<<"Selected root file doesn't exist. Please, generate it. \n";
        return;
    }
    in_f1.close();
    in_f2.close();
    
    
    myf_1 = new TFile(in_1);
    myf_2 = new TFile(in_2);
    
    
    TTree *tree_1 = (TTree*)myf_1->Get("tree");
    TTree *tree_2 = (TTree*)myf_2->Get("tree");
    
    TCanvas *c1 = new TCanvas("c1", "c1",800,600);
    
    tree_1->Draw("vpho_r_pRecoil-nbar_mcP>>histo1(100,-0.2,0.2)","nbar_mcPDG == -2112","goff");
    tree_2->Draw("vpho_r_pRecoil-nbar_mcP>>histo2(100,-0.2,0.2)","nbar_mcPDG == -2112","goff");
    
    delete c1;
    
    TH1 *histo1 = (TH1D*)gDirectory->Get("histo1");
    TH1 *histo2 = (TH1D*)gDirectory->Get("histo2");
    
    double mean_1 = histo1->GetMean();
    double devstd_1 = histo1->GetStdDev();
    double entries_1 = histo1->GetEntries();
    
    double mean_2 = histo2->GetMean();
    double devstd_2 = histo2->GetStdDev();
    double entries_2 = histo2->GetEntries();
    
    
    cout<<"entries_1 = "<<entries_1<<"|mean_1 = "<<mean_1<<"|devstd_1 = "<<devstd_1<<endl;
    cout<<"entries_2 = "<<entries_2<<"|mean_2 = "<<mean_2<<"|devstd_2 = "<<devstd_2<<endl;
    //histo1->Scale(1.0 / histo1->Integral());
    //histo2->Scale(1.0 / histo2->Integral());
    
    double max1 = histo1->GetBinContent(histo1->GetMaximumBin());
    cout<<"max1 = "<<max1<<endl;
    double max2 = histo2->GetBinContent(histo2->GetMaximumBin());
    cout<<"max2 = "<<max2<<endl;
    double max = (max1 > max2) ? max1 : max2;
    cout<<"max = "<<max<<endl;
    
    histo1->SetMaximum(max + 0.1*max);
    histo2->SetMaximum(max + 0.1*max);
    
    histo1->SetLineColor(kBlue);
    histo2->SetLineColor(kRed);
    
    histo1->SetXTitle("#Delta p [#frac{GeV}{c}]");
    histo1->SetYTitle("counts");
    
    TString title = "p recoil - p #bar{n} MC ";
    histo1->SetTitle(title);
    
    
    cout<<"NEntries_1 = "<<histo1->GetEntries()<<endl;
    cout<<"NEntries_2 = "<<histo2->GetEntries()<<endl;
    
    TLegend *leg = new TLegend(0.6,0.6,0.78,0.78);
    leg->AddEntry(histo1,"ISR","l");
    leg->AddEntry(histo2,"NO ISR","l");
    
    TCanvas *tela = new TCanvas("tela", "tela");
    
    histo1->DrawCopy("HIST");
    histo2->DrawCopy("HIST SAMES");
    leg->Draw("SAME");
    
    tela->Update();
    
    tela->SaveAs("../images/kin_delta_p_mc.pdf");
        
}


void recoil_vs_n (TString file_1 = "../../../../vpho_nog_ranked.root")
{
    gStyle->SetOptStat(0);
    //gStyle->SetOptStat(1110);
    
    TFile *myf_1 = nullptr;
    
    ifstream in_f1(file_1);
    
    if(!in_f1)
    {
        cout<<"Selected root file doesn't exist. Please, generate it. \n";
        return;
    }
    in_f1.close();
    
    myf_1 = new TFile(file_1);
    
    TTree *tree_1 = (TTree*)myf_1->Get("tree");
    
    TCanvas *c1 = new TCanvas("c1", "c1",800,600);

    TString var_1 = "vpho_r_pRecoil";
    TString var_2 = "nbar_mcP";
    TString var_3 = "nbar_mcP";
    
    TString drawExpr1 = var_1 + ">>histo1(100,0,7)";
    TString drawExpr2 = var_2 + ">>histo2(100,0,7)";
    TString drawExpr3 = var_3 + ">>histo3(100,0,7)";
    
    tree_1->Draw(drawExpr1,"");
    tree_1->Draw(drawExpr2,"nbar_mcPDG == -2112");
    tree_1->Draw(drawExpr3,"nbar_mcPDG != -2112");
       
    
    delete c1;

    
    TH1D *histo1 = (TH1D*)gDirectory->Get("histo1");
    TH1D *histo2 = (TH1D*)gDirectory->Get("histo2");
    TH1D *histo3 = (TH1D*)gDirectory->Get("histo3");
    
    histo1->SetLineColor(kBlue);
    histo2->SetLineColor(kRed);
    histo3->SetLineColor(kGreen);
    
    double max1 = histo1->GetBinContent(histo1->GetMaximumBin());
    cout<<"max1 = "<<max1<<endl;
    double max2 = histo2->GetBinContent(histo2->GetMaximumBin());
    cout<<"max2 = "<<max2<<endl;
    double max = (max1 > max2) ? max1 : max2;
    cout<<"max = "<<max<<endl;
    
    histo1->SetMaximum(max + 0.1*max);
    histo2->SetMaximum(max + 0.1*max);
    histo3->SetMaximum(max + 0.1*max);
    
    histo1->Scale(1/histo1->Integral());
    histo2->Scale(1/histo2->Integral());
    
    TString title_x = "[#frac{GeV}{c}]";
    histo1->GetXaxis()->SetTitle(title_x);
    histo1->GetYaxis()->SetTitle("counts");
    TString title = "";
    histo1->SetTitle(title);
    
    TLegend *leg = new TLegend(0.6,0.6,0.78,0.78);
    leg->AddEntry(histo1,"Reconstructed recoil momentum","l");
    leg->AddEntry(histo2,"MC momentum of #bar{n} (truth == 1)","l");
    //leg->AddEntry(histo3,"MC momentum of #bar{n} (truth == 0)","l");
    
    TCanvas *tela = new TCanvas("tela", "tela");
    
    histo1->DrawCopy("HIST");
    histo2->DrawCopy("HIST SAMES");
    //histo3->DrawCopy("HIST SAMES");
    leg->Draw("SAME");
    
    TString title_out = "../images/pRecoil_nmcP_PDGsel_nog.pdf";
    tela->SaveAs(title_out);
    
}


void isr (TString file_1 = "../../../../gamma_ISR_tesi.root")
{
    gStyle->SetOptStat(0);
    TFile *myf_1 = new TFile(file_1);
    
    TTree *tree_1 = (TTree*)myf_1->Get("tree");
    
    TCanvas *c1 = new TCanvas("c1", "c1",800,600);

    tree_1->Draw("theta>>h");
   
    delete c1;
    TH1D *h = (TH1D*)gDirectory->Get("h");

    h->SetLineColor(kBlue);
    
    TString title_x = "#theta [rad]";
    h->GetXaxis()->SetTitle(title_x);
    h->GetYaxis()->SetTitle("counts");
    TString title = " ";
    h->SetTitle(title);
    
    TCanvas *tela = new TCanvas("tela", "tela");
    tela->SetLogy();
    
    h->DrawCopy("HIST");

    
    TString title_out = "../images/ISR_theta.pdf";
    tela->SaveAs(title_out);
    
}
