/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package arvoresxml.arvoreB;

import java.util.ArrayList;


public class No {
    private int n;
    private ArrayList<Integer> chave;
    private ArrayList<No> filho;
    private boolean folha;
    
    public No(int n){
        this.chave = new ArrayList<Integer>(n - 1);
        this.filho = new ArrayList<No>(n);
        this.folha = true;
        this.n = 0;
    }
    
    public void setN(int n){
        this.n = n;
    }
    
    public int getN(){
        return n;
    }
    
    public void setChave(ArrayList<Integer> chave){
        this.chave = chave;
    }
    
    public ArrayList<Integer> getChave(){
        return chave;
    }
    
    public void setFilho(ArrayList<No> filho){
        this.filho = filho;
    }
    
    public ArrayList<No> getFilho(){
        return filho;
    }
    
    public void setFolha(boolean folha){
        this.folha = folha;
    }
    
    public boolean eFolha(){
        return folha;
    }
}
