clc,close all, clear all;

% Vetor com os valores reais





y_predito = 270 *ones(1,100);
y_trigo = [272.14	272.06	271.33	271.77	271.97	271.74	271.98	271.69	271.60	271.04	271.70	271.69	271.80	271.42	271.75	271.77	272.00	271.68	271.75	271.74	271.61	271.67	271.59	272.09	271.39	271.65	271.64	272.38	271.62	271.61	271.55	271.77	271.87	272.03	271.85	271.70	271.29	272.27	271.79	272.17	272.00	270.98	272.54	271.03	272.54	271.00	272.85	271.57	271.97	271.83	271.60	271.55	272.07	271.70	271.63	271.74	271.56	272.14	271.82	272.21	271.58	271.58	271.87	271.75	271.83	271.53	272.16	271.43	272.16	271.95	271.94	271.65	272.15	271.68	271.86	271.81	272.36	272.37	271.96	271.73	272.46	271.00	272.35	271.74	271.82	271.87	271.63	271.55	273.01	271.40	272.12	271.14	272.20	271.67	271.59	271.63	271.83	272.36	271.37	272.11];
y_integ = [272.21	271.76	271.12	271.57	271.77	271.53	271.68	271.49	271.39	270.83	271.50	271.49	271.49	271.22	271.55	271.46	271.80	271.37	271.54	271.44	271.31	271.47	271.39	271.89	271.19	271.45	271.43	272.18	271.32	271.41	271.25	271.57	271.67	271.83	271.64	271.39	270.98	271.97	271.59	271.96	271.70	270.77	272.33	270.83	272.34	270.79	272.64	271.36	271.76	271.53	271.39	271.35	271.87	271.49	271.42	271.53	271.35	271.94	271.61	272.00	271.28	271.38	271.67	271.55	271.63	271.33	271.95	271.22	271.95	271.65	271.73	271.44	271.95	271.47	271.66	271.61	272.06	272.17	271.76	271.53	272.26	270.79	272.15	271.53	271.62	271.66	271.42	271.35	272.70	271.09	271.91	270.94	272.00	271.47	271.38	271.42	271.63	272.16	271.16	271.81];


% Vetor com os valores preditos pelo modelo (criando valores aleatórios para demonstração)
% y_predito =  % Adiciona ruído gaussiano aos valores reais

% Cálculo do REMQ
N1 = length(y_trigo);
erro_quadratico1 = (( y_trigo - y_predito).^2);
diferenca1 = ( y_trigo - y_predito);   
REMQ1 = sqrt(erro_quadratico1/N1);


N2 = length(y_integ);
erro_quadratico2 = (( y_integ - y_predito).^2);
diferenca2 = ( y_integ - y_predito);   
REMQ2 = sqrt(erro_quadratico2/N2);

% Plotagem do gráfico

figure;
plot(y_trigo, 'bo-', 'DisplayName', 'Trigonometria');
hold on;
plot(y_predito, 'r*-', 'DisplayName', 'Valor Esperado');
legend('Location', 'best');
title('Gráfico do valor esperado e valor calculado por trigonometria ');
xlabel('Amostras');
ylabel('Valores');
grid on;

figure;
plot(diferenca1, 'g*-', 'DisplayName', 'Diferença');
hold on;
plot(REMQ1, 'r*-', 'DisplayName', 'Erro Quadrático Médio');
legend('Location', 'best');
title('Diferença entre o valor esperado e o calculado por trigonometria');
xlabel('Amostras');
ylabel('Valores');
grid on;

figure;
plot(y_integ, 'bo-',  'DisplayName', 'Integração Numérica');
hold on;
plot(y_predito, 'r*-', 'DisplayName', 'Valor Esperado');
hold on;
legend('Location', 'best');
title('Gráfico do valor esperado e valor calculado por integração numérica ');
xlabel('Amostras');
ylabel('Valores');
grid on;

figure;
plot(diferenca2, 'g*-', 'DisplayName', 'Diferença');
hold on;
plot(REMQ2, 'r*-', 'DisplayName', 'Erro Quadrático Médio');
legend('Location', 'best');
title('Diferença entre o valor esperado e o calculado por integração numérica');
xlabel('Amostras');
ylabel('Valores');
grid on;
