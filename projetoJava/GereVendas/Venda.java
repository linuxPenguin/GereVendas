import java.io.Serializable;
import java.util.Arrays;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Set;
import java.util.List;

/**
 * Classe que representa uma venda.
 * 
 * @author LI3_Grupo1
 * @version 1.0 (06/2016)
 */

public class Venda implements Serializable {
    /** Código do produto vendido */
    private String codigoProduto;
    /** Preço unitário a que o produto foi vendido */
    private double precoUnitario;
    /** Total de unidades vendidas */
    private int unidadesVendidas;
    /** Código do cliente que efetuou a compra */
    private String codigoCliente;
    /** Mês em que se sucedeu a venda */
    private int mes;
    /** Filial onde o produto foi vendido */
    private int filial;
    
    //private TipoVenda tipoVenda;

    /** Construtor por omissão (declarado como private para que o utilizador tenha que especificar os dados de uma venda, de forma a criá-la) */
    private Venda(){}

    /**
     * Cria uma venda com os dados passados como parâmetros.
     * @param codigoProduto Código do produto vendido.
     * @param precoUnitario Preço por unidade de produto.
     * @param unidadesVendidas Número de unidades vendidas.
     * @param codigoCliente Código do cliente que comprou o produto.
     * @param mes Mês da venda.
     * @param filial Filial em que foi realizada a venda.
     */
    public Venda(String codigoProduto, double precoUnitario, int unidadesVendidas, String codigoCliente, int mes, int filial){
        this.unidadesVendidas = unidadesVendidas;
        this.mes = mes;
        this.filial = filial;
        this.codigoProduto = codigoProduto;
        this.codigoCliente = codigoCliente;
        this.precoUnitario = precoUnitario;
        // this.tipoVenda = tipoVenda;
    } 
    
    /**
     * Constrói uma cópia da venda passada como parâmetro.
     * @param v Venda a copiar.
     */
    public Venda(Venda v){        
        this(v.getCodigoProduto(), v.getPrecoUnitario(), v.getUnidadesVendidas(), /*v.getTipoVenda(),*/ v.getCodigoCliente(), v.getMes(), v.getFilial());
    }
    
    /**
     * Testa se esta venda é válida.
     * @return <code>true</code> se esta venda for válida.
     */
    public boolean eValida(){
        return unidadesVendidas >= 0 && mes > 0 && mes < 13 && filial > 0 /*&& filial <= Numero de filiais */ && precoUnitario >= 0;
    }
    
    /**
     * Faz o <i>parsing</i> de uma linha com os dados de uma venda.
     * Cada linha deverá ser formada por:
     * <ol>Código de produto</ol>
     * <ol>Preço unitário decimal</ol>
     * <ol>Número inteiro de unidades compradas</ol>
     * <ol>Tipo de compra (letra N ou P, conforme a compra tenha sido normal ou em promoção)</ol>
     * <ol>Código de cliente</ol>
     * <ol>Mês da compra</ol>
     * <ol>Filial</ol>
     * 
     * @param linha Linha com os dados de uma venda.
     * @return Se a linha passada como parâmetro for válida, é devolvida uma instância de 
     *         Venda criada a partir da linha, se não é devolvido <code>null</code>.
     */
    public static Venda parseLinhaVenda(String linha){
        int unidadesVendidas, mes, filial;
        String codigoProduto, codigoCliente;
        double precoUnitario;
        /*TipoVenda tipoVenda;*/
        String[] campos = linha.split(" ");

        codigoProduto = campos[0];
        codigoCliente = campos[4];

        try{
            unidadesVendidas = Integer.parseInt(campos[2]);
            mes = Integer.parseInt(campos[5]);
            filial = Integer.parseInt(campos[6]);
            precoUnitario = Double.parseDouble(campos[1]);
            /*tipoVenda = TipoVenda.fromString(campos[3]);*/
        }
        catch(NumberFormatException | NullPointerException /*| TipoVendaInvalidoException*/ e){
            return null;
        }
        return new Venda(codigoProduto, precoUnitario, unidadesVendidas, /* tipoVenda, */ codigoCliente, mes, filial);
    }
    
    /**
     * Faz o <i>parsing</i> de um <pre>{@code ArrayList<String>}</pre> para um <pre>{@code ArrayList<Venda>}</pre>.
     * @param linhas Lista de Strings, em cada String tem os dados de uma venda.
     * 
     * @return <code>ArrayList</code> de objetos do tipo Venda, resultante do <i>parsing</i> de <code>linhas</code> 
     */
    public static ArrayList<Venda> parseAllLinhas(ArrayList<String> linhas){
        ArrayList<Venda> vendas = new ArrayList<>();

        for(String str : linhas){
            Venda v = parseLinhaVenda(str);
            if(v != null)
                vendas.add(v);
        }
        return vendas;
    }
    
    /**
     * Faz o <i>parsing</i> de um <pre>{@code ArrayList<String>}</pre> para um <pre>{@code HashSet<Venda>}</pre>.
     * @param linhas Lista de Strings, em cada String tem os dados de uma venda.
     * 
     * @return <code>ArrayList</code> de objetos do tipo Venda, resultante do <i>parsing</i> de <code>linhas</code> 
     */
    public static HashSet<Venda> parseAllLinhasToSet(ArrayList<String> linhas){
        HashSet<Venda> vendas = new HashSet<>();

        for(String str : linhas){
            Venda v = parseLinhaVenda(str);
            if(v != null)
                vendas.add(v);
        }
        return vendas;
    }
    
    /** Getters */
    
    /**
     * Devolve o código de produto comercializado nesta na venda.
     * @return Código do produto vendido.
     */
    public String getCodigoProduto(){
        return codigoProduto;
    }
    
    /**
     * Preço unitário a que foi efetuada desta venda.
     * @return Preço unitário desta venda.
     */
    public double getPrecoUnitario(){
        return precoUnitario;
    }
    
    /**
     * Devolve o número de unidades vendidas nesta venda.
     * @return Unidades vendidas nesta venda.
     */
    public int getUnidadesVendidas(){
        return unidadesVendidas;
    }
    
    /**
     * Devolve o código do cliente que e
     */
    public String getCodigoCliente(){
        return codigoCliente;
    }
    
    /**
     * Devolve o mês em que se realizou esta venda.
     * @return Mês desta venda.
     */
    public int getMes(){
        return mes;
    }
    
    /**
     * Devolve a filial em que se realizou esta venda.
     * @return Filial onde decorreu esta venda.
     */
    public int getFilial(){
        return filial;
    }

    

   

    

    /*public TipoVenda getTipoVenda(){
    return tipoVenda;
    }*/

    /* nao definimos setters porque queremos que as instancias desta class sejam imutaveis */

    /* Metodos standard */
    public Venda clone(){
        return new Venda(this);
    }
    
    public boolean equals(Object o){
        if(this == o)
            return true;
        if(o == null || this.getClass() != o.getClass())
            return false;
        
        Venda v = (Venda) o;
        return this.unidadesVendidas == v.unidadesVendidas && this.mes == v.mes && this.filial == v.filial &&
               (this.codigoProduto == null ? v.codigoProduto == null : this.codigoProduto.equals(v.codigoProduto)) &&
               (this.codigoCliente == null ? v.codigoCliente == null : this.codigoCliente.equals(v.codigoCliente)) &&
               this.precoUnitario == v.precoUnitario;
        /* && this.tipoVenda == v.tipoVenda;*/
    }
    public int hashCode(){
        return Arrays.hashCode(new Object[]{unidadesVendidas, mes, filial, codigoProduto, codigoCliente, precoUnitario/*, tipoVenda*/});
    }

    public String toString(){
        StringBuilder sb = new StringBuilder();
        String separador = System.getProperty("line.separator");
        
        sb.append("-> Venda" + separador);
        sb.append("Unidades vendidas: " + unidadesVendidas + separador);
        sb.append("Mes: " + mes + separador);
        sb.append("Filial: " + filial + separador);
        sb.append("Código de produto: " + codigoProduto + separador);
        sb.append("Código de cliente: " + codigoCliente + separador);
        sb.append("Preço unitário" + precoUnitario + separador);
        /*sb.append("\ntipoVenda: ");
        sb.append(tipoVenda + "\n");*/
        return sb.toString();
    }




}

