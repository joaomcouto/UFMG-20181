//Módulo que calcula o mdc entre a e b
module mdc(clk, a, b, g);

  input clk; // o clock permitirá com que o nosso circuito só fique operacional durante uma borda de subida
  input[15:0] a, b; // os inputs a e b, serão números de 16 bits (por default são do tipo wire)
  output[15:0] g; // o nosso output também terá 16 bits (por default é do tipo wire)
  
  reg[15:0] resto; // registrador de 16 bits para guardar o resto entre 'a' e 'b'
  reg[15:0] A; // registrador de 16 bits para guardar o valor de 'a' durante a computação
  reg[15:0] B; // registrador de 16 bits para guardar o valor de 'b' durante a computação
  
  // Nossa lógica sempre será aplicada durante a borda de subida do clock
  always @(posedge clk) begin
      A = a;
      B = b;
      resto = A % B;
      
      // Aplicação do algoritmo euclidiano para o cálculo do mdc(a, b)
      // No caso, teremos que mdc(a, b) = mdc(b, r) = ... onde a = bq + r com 0 <= r < b
      while(resto != 0) begin
        A = B;
        B = resto;
        resto = A % B;
      end
  end
  
  // Damos como output o valor do mdc(a, b)... por conta da forma com que o código dentro do while
  // foi feito, temos que falar que mdc será o valor que tiver em B depois dos cálculos e não o de A
  assign g = B;
endmodule


// Módulo de teste para as formas de onda
module teste;
 
  reg clk; // nosso clock
  reg[15:0] a, b; // variáveis de input
  wire[15:0] g; // saída: g = mdc(a, b)

  // Instanciação do módulo de mdc passando o clock, a, b, e o mdc a ser computado
  mdc mdc1(clk, a, b, g);
  
  // Inicialmente começamos com o clock = 0
  initial begin
    clk = 1'b0;
  end
  
  // Sempre a cada 10s o nosso clock fica o clock negado
  always begin
    #10 clk = ~clk;
  end
  
  // Bloco de instrução para a plotagem em forma de onda
  initial begin
    $dumpfile("mdc.vcd"); // arquivo .vcd a ser criado
    $dumpvars;
    
    // Teste 1: no tempo 0 -> a = 8, b = 2 (g deve ser 2 após a borda de subida do clock)
    #0 a = 4'b1000;
    #0 b = 4'b0010;
    
    // Teste 2: no tempo 10 -> a = 12, no tempo 12 -> b = 4 (g deve ser 4 após a borda de subida do clock)
    #10 a = 4'b1100;
    #2 b = 4'b0100;
    
    // Teste 3: no tempo 36 -> a = 15, b = 5 (g deve ser 5 após a borda de subida do clock)
    #24 a = 4'b1111;
    #0 b = 4'b0101;

    // Teste 4: no tempo 48 -> b = 3 (g deve ser mdc(15, 3) = 3 após a borda de subida do clock)
    #12 b = 4'b0011;

    // Teste 5: no tempo 66 -> b = 2 (g deve ser mdc(15, 2) = 1 após a borda de subida do clock)
    #18 b = 4'b0010;

    // Teste 6: no tempo 76 -> b = 1 (g deve ser mdc(15, 1) = 1 após a borda de subida do clock)
    #10 b = 4'b0001;

    // Teste 7: no tempo 86 -> a = 170 e b = 4 (g deve ser 2 após a borda de subida do clock)
    #10 a = 8'b1010_1010;
    #0 b = 4'b0100;
  end

  // Nosso teste durará 100s
  initial begin
    #100 $finish;
  end
endmodule
