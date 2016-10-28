library IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.NUMERIC_STD.ALL;

-- fifo taken from http://www.deathbylogic.com/2013/07/vhdl-standard-fifo/

entity FIFO is
    generic (
        DATA_WIDTH: integer;
        QUEUE_SIZE: integer
    );
    port (
        clk:      in  std_logic;
        rst:      in  std_logic;
        rd_wr:    in  std_logic;  -- read ('1') and write ('0') control
        enable:   in  std_logic;  -- habilita ('0') a leitura ou escrita na fila
        full:     out std_logic := '0';  -- full queue ('1')
        empty:    out std_logic := '0';  -- empty queue ('1')
        data_in:  in  std_logic_vector(DATA_WIDTH - 1 downto 0);
        data_out: out std_logic_vector(DATA_WIDTH - 1 downto 0)
    );
end entity FIFO;

architecture Behavioral of FIFO is
begin
     -- implementation of FIFO
    fifo_proc : process (clk)
        type FIFO_Memory is array (0 to QUEUE_SIZE - 1) of std_logic_vector(DATA_WIDTH - 1 downto 0);
        variable Memory : FIFO_Memory;
        
        variable Head : natural range 0 to QUEUE_SIZE - 1 := 0;
        variable Tail : natural range 0 to QUEUE_SIZE - 1 := 0;

        variable Looped : boolean := false;
    begin
        if rising_edge(clk) then
            if rst = '0' then
                Head := 0;
                Tail := 0;
                
                Looped := false;
                
                full  <= '0';
                empty <= '1';
            else
                if (rd_wr = '1' and enable = '0') then
                    if ((Looped = true) or (Head /= Tail)) then
                        -- Update data output
                        data_out <= Memory(Tail);
                        
                        -- Update Tail pointer as needed
                        if (Tail = QUEUE_SIZE - 1) then
                            Tail := 0;
                            
                            Looped := false;
                        else
                            Tail := Tail + 1;
                        end if;
                        
                    end if;
                end if;
                
                if (rd_wr = '0' and enable = '0') then
                    if ((Looped = false) or (Head /= Tail)) then
                        -- Write Data to Memory
                        Memory(Head) := data_in;
                        
                        -- Increment Head pointer as needed
                        if (Head = QUEUE_SIZE - 1) then
                            Head := 0;
                            
                            Looped := true;
                        else
                            Head := Head + 1;
                        end if;
                    end if;
                end if;
                
                -- Update empty and full flags
                if (Head = Tail) then
                    if Looped then
                        full <= '1';
                    else
                        empty <= '1';
                    end if;
                else
                    empty   <= '0';
                    full    <= '0';
                end if;
            end if;
        end if;
    end process;
end Behavioral; 