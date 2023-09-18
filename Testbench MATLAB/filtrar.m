function [y] = filtrar(x)
    load('bandpass_2828_5657.mat'); % Carga los coeficientes del filtro

    y = zeros(size(x, 1), 1); % Genera el vector para la salida

    % Filtra la entrada, una etapa a la vez 
    for k = 1:3
        % Saca los coeficientes necesarios para cada etapa del filtro
        a_1 = coefs(k, 5);
        a_2 = coefs(k, 6);

        w_2 = 0; % w_2(-1)
        w_1 = 0; % w_1(0)

        % Calcula las muestras de la salida y actualiza los valores de w_1 y w_2
        for n = 1:size(x, 1)
            y(n) = (x(n) + w_1);

            w_1 = w_2 - a_1*y(n);
            w_2 = -1*(x(n) + a_2*y(n));
        end

        % Multiplica la salida por la ganancia correspondiente
        % y asigna el resultado a la entrada para procesarlo
        % en la siguiente etapa del filtro
        x = gains(k) * y;
    end
    
    y = x; % Asigna el valor de la salida cuando se acaba el procesamiento
end