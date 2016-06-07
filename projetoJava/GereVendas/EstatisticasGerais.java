import java.util.Arrays;
import java.io.Serializable;

public class EstatisticasGerais implements Serializable{
    private int[] totalCompras;
    private double[][] totalFaturado;
    private int[] totalCliDistintos;

    public EstatisticasGerais(){
        totalCompras = new int[Constantes.N_MESES+1];
        totalFaturado = new double[Constantes.N_MESES+1][Constantes.N_FILIAIS+1];
        totalCliDistintos = new int[Constantes.N_MESES+1];
    }

    public EstatisticasGerais(int[] totalCompras, double[][] totalFaturado, int[] totalCliDistintos){
        this();
        setTotalCompras(totalCompras);
        setTotalFaturado(totalFaturado);
        setTotalCliDistintos(totalCliDistintos);
    }
    
    public EstatisticasGerais(EstatisticasGerais est){
        this(est.getTotalCompras(), est.getTotalFaturado(), est.getTotalCliDistintos());
    }

    public int[] getTotalCompras() { return Arrays.copyOf(totalCompras, totalCompras.length); }
    
    public double[][] getTotalFaturado() { 
        double[][] copia = new double[Constantes.N_MESES+1][Constantes.N_FILIAIS+1];
        
        for(int i = 1; i < totalFaturado.length; i++)
            copia[i] = Arrays.copyOf(totalFaturado[i], totalFaturado[i].length);
        return copia;
    }
    
    public int[] getTotalCliDistintos() { return Arrays.copyOf(totalCliDistintos, totalCliDistintos.length); }
    // length do nosso array para o caso do array argumento ser maior
    public void setTotalCompras(int[] totalCompras) {
        this.totalCompras = Arrays.copyOf(totalCompras, totalCompras.length);
    }
    
    public void setTotalFaturado(double[][] totalFaturado) { 
        for(int i = 1; i < totalFaturado.length; i++)
            this.totalFaturado[i] = Arrays.copyOf(totalFaturado[i], totalFaturado[i].length);
    }
    
    public void setTotalCliDistintos(int[] totalCliDistintos) {
        this.totalCliDistintos = Arrays.copyOf(totalCliDistintos,    totalCliDistintos.length);
    }

    /* Adicionar alguns métodos para atualizar a as variaveis por mês ou assim, se facilitar */

    public boolean equals(Object o){
        if(this == o)
            return true;
        if((o == null) || (this.getClass() != o.getClass()))
            return false;

        EstatisticasGerais est = (EstatisticasGerais) o;
        boolean res = true;
        double[][] totalFaturado = est.getTotalFaturado();
        
        for(int i = 1; i <= Constantes.N_MESES && res; i++)
            res = Arrays.equals(this.totalFaturado[i], totalFaturado[i]);

        return res && Arrays.equals(totalCompras, est.getTotalCompras()) &&
               Arrays.equals(totalCliDistintos, est.getTotalCliDistintos());
    }

    public EstatisticasGerais clone(){
        return new EstatisticasGerais(this);
    }

    public String toString(){
        StringBuilder str = new StringBuilder();
        return str.toString();
    }
}
