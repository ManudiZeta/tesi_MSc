#include "TFile.h"
#include <Riostream.h>
#include "TCanvas.h"
#include "TH1D.h"
#include "TTree.h"
#include "TH2F.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TString.h"
static TString nome = "isr";
static TString file_str = "../../../../vpho_isr_ranked.root";

void cluster()
{
    
    gStyle->SetOptStat(0);
    
    double alpha_num [10]  = {0.35,0.30,0.25,0.20,0.15,0.10,0.075,0.05,0.04,0.03};
    
    TString alpha_str [10] = {"0.35","0.30","0.25","0.20","0.15","0.10","0.075","0.05","0.04","0.03"};
    
    TString var[8] = {"clusterE","clusterLAT","clusterNHits","clusterSecondMoment","clusterE1E9","clusterE9E21","clusterAbsZernikeMoment51","clusterAbsZernikeMoment40"};
    TString lim [8] = {"(100,0,6)","(128,0,1)","(80,-0.5,79.5)","(256,0,40)","(128,0,1)","(128,0,1)","(128,0,1.2)","(128,0,1.7)"};
    TString names[8] = {"E","Lateral Momentum","N Hits","Second Moment","E1E9","E9E21","Zernike Moment 51","Zernike Moment 40"};
    TString um [8] = {" [GeV]","","","","","","",""};
    TFile *myf_1 = new TFile(file_str);
    
    for(int j = 0; j<1; j ++)
    {
        for(int i = 0; i<8; i++)
        {
            TTree *tree = (TTree*)myf_1->Get("tree");
            
            TCanvas *c1 = new TCanvas("c1", "c1",800,600);
            
            tree->Draw("nbar_" + var[i] + ">>h1" + lim[i],"" ,"goff");
            tree->Draw("nbar_" + var[i] + ">>h2" + lim[i],"nbar_mcPDG == -2112","goff");
            
            
            TH1 *h1 = (TH1*)gDirectory->Get("h1");
            TH1 *h2 = (TH1*)gDirectory->Get("h2");
            cout<<"h1 entries: "<<h1->GetEntries()<<endl;
            cout<<"h2 entries: "<<h2->GetEntries()<<endl;
            
            delete c1;
            
            
            h1->SetLineColor(kBlue);
            h2->SetLineColor(kRed);
            //f->SetLineColor(kRed);
            
            h1->GetXaxis()->SetTitle(names[i] + um[i]);
            h1->GetYaxis()->SetTitle("counts");
            
            
            TString title = var[i];
            h1->SetTitle(title);
            
            TLegend *leg = new TLegend(0.70, 0.85, 0.95, 0.95);
            leg->AddEntry(h1,"All #bar{n} candidates","l");
            leg->AddEntry(h2,"#bar{n} MC truth ID ","l");
            
            
            TCanvas *tela = new TCanvas("tela", "tela");
            
            h1->DrawCopy("HIST");
            h2->DrawCopy("SAME");
            leg->Draw("SAME");
            
            TString title_out = "../images/" + var[i] + "_" + nome + ".pdf";
            tela->SaveAs(title_out);
            
            delete tree;
            delete tela;
        }
    }
}

void ancestor_cluster()
{
    
    gStyle->SetOptStat(0);
    
    double alpha_num [10]  = {0.35,0.30,0.25,0.20,0.15,0.10,0.075,0.05,0.04,0.03};
    
    TString alpha_str [10] = {"0.35","0.30","0.25","0.20","0.15","0.10","0.075","0.05","0.04","0.03"};
    
    TString var[8] = {"clusterE","clusterLAT","clusterNHits","clusterSecondMoment","clusterE1E9","clusterE9E21","clusterAbsZernikeMoment51","clusterAbsZernikeMoment40"};
    TString lim [8] = {"(100,0,6)","(128,0,1)","(80,-0.5,79.5)","(256,0,40)","(128,0,1)","(128,0,1)","(128,0,1.2)","(128,0,1.7)"};
    TString names[8] = {"E","Lateral Momentum","N Hits","Second Moment","E1E9","E9E21","Zernike Moment 51","Zernike Moment 40"};
    TString um [8] = {" [GeV]","","","","","","",""};
    TFile *myf_1 = new TFile(file_str);
    
    for(int j = 0; j<1; j ++)
    {
        for(int i = 0; i<8; i++)
        {
            TTree *tree = (TTree*)myf_1->Get("tree");
            
            TCanvas *c1 = new TCanvas("c1", "c1",800,600);
            
            tree->Draw("nbar_" + var[i] + ">>h1" + lim[i],"nbar_mcPDG == -2112" ,"goff");
            tree->Draw("nbar_" + var[i] + ">>h2" + lim[i],"TMath::IsNaN(nbar_mcPDG)","goff");
            tree->Draw("nbar_" + var[i] + ">>h3" + lim[i],"nbar_hasAncestor_2112_1 == 0","goff");
            tree->Draw("nbar_" + var[i] + ">>h4" + lim[i],"nbar_hasAncestor_2112_1 != 0 ","goff");
            
            
            TH1 *h1 = (TH1*)gDirectory->Get("h1");
            TH1 *h2 = (TH1*)gDirectory->Get("h2");
            TH1 *h3 = (TH1*)gDirectory->Get("h3");
            TH1 *h4 = (TH1*)gDirectory->Get("h4");
            
            cout<<"h1 entries: "<<h1->GetEntries()<<endl;
            cout<<"h2 entries: "<<h2->GetEntries()<<endl;
            cout<<"h3 entries: "<<h3->GetEntries()<<endl;
            cout<<"h4 entries: "<<h4->GetEntries()<<endl;
            
            delete c1;
            
            
            h1->SetLineColor(kRed);
            h2->SetLineColor(kBlue);
            h3->SetLineColor(kGreen);
            h4->SetLineColor(kOrange);
            //f->SetLineColor(kRed);
            h1->SetLineWidth(3);
            h2->SetLineWidth(3);
            h3->SetLineWidth(3);
            h4->SetLineWidth(3);
            
            h4->GetXaxis()->SetTitle(names[i] + um[i]);
            h4->GetYaxis()->SetTitle("counts");
            
            
            TString title = var[i];
            h4->SetTitle(title);
            TLegend *leg;
            
            if (var[i] == "clusterE9E21" ||var[i] ==  "clusterAbsZernikeMoment40")
            {
                leg = new TLegend (0.10, 0.75, 0.40, 0.90);
            }
            else
            {
                leg = new TLegend(0.60, 0.75, 0.90, 0.90);
            }
            
            leg->AddEntry(h1,"#bar{n} MC truth ID","l");
            leg->AddEntry(h2,"#bar{n} not associated ","l");
            leg->AddEntry(h3,"#bar{n} with no #bar{n} relative","l");
            leg->AddEntry(h4,"#bar{n} with #bar{n} relative","l");
            
            
            TCanvas *tela = new TCanvas("tela", "tela");
            
            h4->DrawCopy("HIST");
            h2->DrawCopy("SAME");
            h3->DrawCopy("SAME");
            h1->DrawCopy("SAME");
            leg->Draw("SAME");
            
            TString title_out = "../images/" + var[i] + "_" + nome + "_ancestor.pdf";
            tela->SaveAs(title_out);
            
            delete tree;
            delete tela;
        }
    }
}

void NHits_cluster()
{
    
    gStyle->SetOptStat(0);
    
    double alpha_num [10]  = {0.35,0.30,0.25,0.20,0.15,0.10,0.075,0.05,0.04,0.03};
    
    TString alpha_str [10] = {"0.35","0.30","0.25","0.20","0.15","0.10","0.075","0.05","0.04","0.03"};
    
    TString var[8] = {"clusterE","clusterLAT","clusterNHits","clusterSecondMoment","clusterE1E9","clusterE9E21","clusterAbsZernikeMoment51","clusterAbsZernikeMoment40"};
    TString lim [8] = {"(100,0,6)","(128,0,1)","(80,-0.5,79.5)","(256,0,40)","(128,0,1)","(128,0,1)","(128,0,1.2)","(128,0,1.7)"};
    //TString lim [8] = {"(100,0,6)","(256,0,1)","(80,-0.5,79.5)","(512,0,40)","(256,0,1)","(256,0,1)","(256,0,1.2)","(256,0,1.7)"};
    TString names[8] = {"E","Lateral Momentum","N Hits","Second Moment","E1E9","E9E21","Zernike Moment 51","Zernike Moment 40"};
    TString um [8] = {" [GeV]","","","","","","",""};
    TFile *myf_1 = new TFile(file_str);
    
    for(int j = 0; j<1; j ++)
    {
        for(int i = 0; i<8; i++)
        {
            TTree *tree = (TTree*)myf_1->Get("tree");
            
            TCanvas *c1 = new TCanvas("c1", "c1",800,600);
            
            tree->Draw("nbar_" + var[i] + ">>h1" + lim[i],"nbar_clusterNHits>10" ,"goff");
            tree->Draw("nbar_" + var[i] + ">>h2" + lim[i],"nbar_mcPDG == -2112 && nbar_clusterNHits>10","goff");
            
            
            TH1 *h1 = (TH1*)gDirectory->Get("h1");
            TH1 *h2 = (TH1*)gDirectory->Get("h2");
            cout<<"h1 entries: "<<h1->GetEntries()<<endl;
            cout<<"h2 entries: "<<h2->GetEntries()<<endl;
            
            delete c1;
            
            
            h1->SetLineColor(kBlue);
            h2->SetLineColor(kRed);
            //f->SetLineColor(kRed);
            
            h1->GetXaxis()->SetTitle(names[i] + um[i]);
            h1->GetYaxis()->SetTitle("counts");
            
            
            TString title = var[i] + " with cut clusterNHits > 10";
            h1->SetTitle(title);
            
            TLegend *leg;
            
            if (var[i] == "clusterE9E21" ||var[i] ==  "clusterAbsZernikeMoment40" ||var[i] ==  "clusterLAT" )
            {
                leg = new TLegend (0.10, 0.75, 0.40, 0.90);
            }
            else
            {
                leg = new TLegend(0.60, 0.75, 0.90, 0.90);
            }
            
            leg->AddEntry(h1,"All #bar{n} candidates","l");
            leg->AddEntry(h2,"#bar{n} MC truth ID ","l");
            
            
            TCanvas *tela = new TCanvas("tela", "tela");
            
            h1->DrawCopy("HIST");
            h2->DrawCopy("SAME");
            leg->Draw("SAME");
            
            TString title_out = "../images/" + var[i] + "_hitscut_"+ nome +".pdf";
            tela->SaveAs(title_out);
            
            delete tree;
            delete tela;
        }
    }
}

void region_NHits_cluster()
{
    
    gStyle->SetOptStat(0);
    
    double alpha_num [10]  = {0.35,0.30,0.25,0.20,0.15,0.10,0.075,0.05,0.04,0.03};
    
    TString alpha_str [10] = {"0.35","0.30","0.25","0.20","0.15","0.10","0.075","0.05","0.04","0.03"};
    
    TString var[8] = {"clusterE","clusterLAT","clusterNHits","clusterSecondMoment","clusterE1E9","clusterE9E21","clusterAbsZernikeMoment51","clusterAbsZernikeMoment40"};
    TString lim [8] = {"(64,0,6)","(64,0,1)","(80,-0.5,79.5)","(64,0,40)","(64,0,1)","(64,0,1)","(64,0,1.2)","(96,0,1.7)"};
    TString names[8] = {"E","Lateral Momentum","N Hits","Second Moment","E1E9","E9E21","Zernike Moment 51","Zernike Moment 40"};
    TString um [8] = {" [GeV]","","","","","","",""};
    TFile *myf_1 = new TFile(file_str);
    
    for(int j = 0; j<1; j ++)
    {
        for(int i = 0; i<8; i++)
        {
            TTree *tree = (TTree*)myf_1->Get("tree");
            
            TCanvas *c1 = new TCanvas("c1", "c1",800,600);
            
            tree->Draw("nbar_" + var[i] + ">>h1" + lim[i],"nbar_clusterNHits>10 && nbar_clusterReg == 1" ,"goff");
            tree->Draw("nbar_" + var[i] + ">>h2" + lim[i],"nbar_clusterNHits>10 && nbar_clusterReg == 2","goff");
            tree->Draw("nbar_" + var[i] + ">>h3" + lim[i],"nbar_clusterNHits>10 && nbar_clusterReg == 3","goff");
            
            
            TH1 *h1 = (TH1*)gDirectory->Get("h1");
            TH1 *h2 = (TH1*)gDirectory->Get("h2");
            TH1 *h3 = (TH1*)gDirectory->Get("h3");
            
            cout<<"h1 entries: "<<h1->GetEntries()<<endl;
            cout<<"h2 entries: "<<h2->GetEntries()<<endl;
            cout<<"h3 entries: "<<h3->GetEntries()<<endl;
            
            delete c1;
            
            
            h1->SetLineColor(kBlue);
            h2->SetLineColor(kRed);
            h3->SetLineColor(kGreen);
            //f->SetLineColor(kRed);
            
            h1->SetLineWidth(3);
            h2->SetLineWidth(3);
            h3->SetLineWidth(3);
            
            
            
            h2->GetXaxis()->SetTitle(names[i] + um[i]);
            h2->GetYaxis()->SetTitle("counts");
            
            h1->Scale(1/h1->GetEntries());
            h2->Scale(1/h2->GetEntries());
            h3->Scale(1/h3->GetEntries());
            
            double max1 = h1->GetBinContent(h1->GetMaximumBin());
            double max2 = h2->GetBinContent(h2->GetMaximumBin());
            double max3 = h3->GetBinContent(h3->GetMaximumBin());
            
            double max = max1;
            if (max2 > max) max = max2;
            if (max3 > max) max = max3;
            
            h1->SetMaximum(max * 1.1);
            h2->SetMaximum(max * 1.1);
            h3->SetMaximum(max * 1.1);
            
            
            TString title = var[i] + " with cut clusterNHits > 10";
            h2->SetTitle(title);
            TLegend *leg;
            if (var[i] == "clusterE9E21" ||var[i] ==  "clusterAbsZernikeMoment40" ||var[i] ==  "clusterLAT" )
            {
                leg = new TLegend (0.10, 0.75, 0.40, 0.90);
            }
            else
            {
                leg = new TLegend(0.60, 0.75, 0.90, 0.90);
            }
            leg->AddEntry(h1,"Forward Region","l");
            leg->AddEntry(h2,"Barrel region ","l");
            leg->AddEntry(h3,"Backward region ","l");
            
            TCanvas *tela = new TCanvas("tela", "tela");
            
            h2->DrawCopy("HISTO");
            h1->DrawCopy("HISTO SAME");
            h3->DrawCopy("HISTO SAME");
            leg->Draw("HISTO SAME");
            
            TString title_out = "../images/" + var[i] +"_"+ nome + "_"+"region.pdf";
            tela->SaveAs(title_out);
            
            delete tree;
            delete tela;
        }
    }
}


void region()
{
    
    gStyle->SetOptStat(0);
    TFile *myf_1 = new TFile(file_str);
    TTree *tree = (TTree*)myf_1->Get("tree");
    
    TCanvas *c1 = new TCanvas("c1", "c1",800,600);
    
    tree->Draw("nbar_clusterReg>>h1(100,0,5)","nbar_clusterNHits>10" ,"goff");
    //tree->Draw("nbar_clusterReg>>h2(100,0,5)","nbar_clusterNHits>10 && nbar_clusterReg == 2" ,"goff");
    //tree->Draw("nbar_clusterReg>>h3(100,0,5)","nbar_clusterNHits>10 && nbar_clusterReg == 3" ,"goff");
    
    TH1 *h1 = (TH1*)gDirectory->Get("h1");
    //TH1 *h2 = (TH1*)gDirectory->Get("h2");
    //TH1 *h3 = (TH1*)gDirectory->Get("h3");
    
    cout<<"h1 entries: "<<h1->GetEntries()<<endl;
    //cout<<"h2 entries: "<<h2->GetEntries()<<endl;
    //cout<<"h3 entries: "<<h3->GetEntries()<<endl;
    
    delete c1;
    
    
    h1->SetLineColor(kBlue);
    //h2->SetLineColor(kRed);
    //h3->SetLineColor(kGreen);
    //f->SetLineColor(kRed);
    
    h1->GetXaxis()->SetTitle("cluster region");
    h1->GetYaxis()->SetTitle("counts");
            
    TString title = "";
    h1->SetTitle(title);
    
   /*
    TLegend *leg = new TLegend(0.70, 0.85, 0.95, 0.95);
    leg->AddEntry(h1,"Forward Region","l");
    leg->AddEntry(h2,"Barrel region ","l");
    leg->AddEntry(h3,"Backward region ","l");
    */
    TCanvas *tela = new TCanvas("tela", "tela");
    
    h1->DrawCopy("HISTO");
    //h2->DrawCopy("HISTO SAME");
    //h3->DrawCopy("HISTO SAME");
    //leg->Draw("HISTO SAME");
    
    TString title_out = "../images/cluster_region.pdf";
    tela->SaveAs(title_out);
    
    delete tree;
    delete tela;
    
}
