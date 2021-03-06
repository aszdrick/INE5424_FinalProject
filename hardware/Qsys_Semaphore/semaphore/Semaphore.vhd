----------------------------------------------------------
-- Company: Software and Hardware Integration Lab - LISHA
-- Engineer: Hugo Marcondes
-- Design Name: Hardware Semaphore
-- Project Name: Hybrid Hw/Sw Components
----------------------------------------------------------
----------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
entity semaphore is
    generic (
        C_MAX_SEMAPHORES: integer := 8; -- Must lower than 2^C_VALUE_WIDTH
        C_FIFO_SIZE:      integer := 4;
        C_VALUE_WIDTH:    integer := 8;
        C_FIFO_WIDTH:     integer := 32
    );
    port (
        p_clk:      in  std_logic;
        p_rst:      in  std_logic;
        p_cmd:      in  std_logic_vector(2 downto 0);
        p_addr:     in  std_logic_vector(C_VALUE_WIDTH - 1 downto 0);
        p_data_in:  in  std_logic_vector(C_FIFO_WIDTH - 1 downto 0);
        p_data_out: out std_logic_vector(C_FIFO_WIDTH - 1 downto 0);
        p_status:   out std_logic_vector(4 downto 0)
    );
end semaphore;

architecture Behavioral of semaphore is
    -- Types
    type t_memory is array(0 to C_MAX_SEMAPHORES - 1) of std_logic_vector(C_VALUE_WIDTH - 1 downto 0);
    type t_fifo_data is array (0 to C_MAX_SEMAPHORES - 1) of std_logic_vector(C_FIFO_WIDTH - 1 downto 0);
    type t_state is (idle, createCmd, destroyCmd, upCmd, downCmd, resumeThr);
    
    -- Semaphore Commands
    constant C_CMD_CREATE:  std_logic_vector(2 downto 0) := "001";
    constant C_CMD_DESTROY: std_logic_vector(2 downto 0) := "010";
    constant C_CMD_DOWN:    std_logic_vector(2 downto 0) := "011";
    constant C_CMD_UP:      std_logic_vector(2 downto 0) := "100";
    
    -- State Machine Type
    signal s_state: t_state;
    
    -- Internal Memory
    signal s_bitmap: std_logic_vector(0 to C_MAX_SEMAPHORES - 1) := (others => '0');
    signal s_memory: t_memory;
    
    -- Internal Registers
    signal s_free:     integer range C_MAX_SEMAPHORES - 1 downto 0 := 0;
    signal s_idx:      integer range C_MAX_SEMAPHORES - 1 downto 0 := 0;
    signal s_thread:   std_logic_vector(C_FIFO_WIDTH - 1 downto 0) := (others => '0');
    signal s_value:    std_logic_vector(C_VALUE_WIDTH - 1 downto 0) := (others => '0');
    
    -- Internal Registers
    signal s_addr:     std_logic_vector(C_VALUE_WIDTH - 1 downto 0) := (others => '0');
    signal s_data_in:  std_logic_vector(C_FIFO_WIDTH - 1 downto 0) := (others => '0');
    --signal s_data_out: std_logic_vector(C_FIFO_WIDTH - 1 downto 0) := (others => '0');
    signal s_resume:   std_logic := '0';
    signal s_block:    std_logic := '0';
    signal s_full:     std_logic := '0';
    signal s_error:    std_logic := '0';
    signal s_done:     std_logic := '0';

    -- FIFO Signals
    signal s_fifo_rd_wr:  std_logic_vector(0 to C_MAX_SEMAPHORES - 1) := (others => '0');
    signal s_fifo_enable: std_logic_vector(0 to C_MAX_SEMAPHORES - 1) := (others => '0');
    signal s_fifo_full:   std_logic_vector(0 to C_MAX_SEMAPHORES - 1) := (others => '0');
    signal s_fifo_empty:  std_logic_vector(0 to C_MAX_SEMAPHORES - 1) := (others => '0');
    signal s_fifo_data_in:  t_fifo_data;
    signal s_fifo_data_out: t_fifo_data;
    
    component FIFO is
        generic (
            DATA_WIDTH: integer;
            QUEUE_SIZE: integer
        );
        port (
            clk:      in  std_logic;
            rst:      in  std_logic;
            rd_wr:    in  std_logic;  -- read ('1') and write ('0') control
            enable:   in  std_logic;  -- habilita ('0') a leitura ou escrita na fila
            full:     out std_logic;  -- full queue ('1')
            empty:    out std_logic;  -- empty queue ('1')
            data_in:  in  std_logic_vector(DATA_WIDTH - 1 downto 0);
            data_out: out std_logic_vector(DATA_WIDTH - 1 downto 0)
        );
    end component FIFO;
	
    -- Create the Semaphore FIFOs
begin
    fifo_array : for sem_index in 0 to C_MAX_SEMAPHORES - 1 generate
        begin fifo_component : component FIFO
            generic map (
                DATA_WIDTH => C_FIFO_WIDTH,
                QUEUE_SIZE => C_FIFO_SIZE
            )
            port map (
                clk => p_clk,
                rst => p_rst,
                rd_wr => s_fifo_rd_wr(sem_index),
                enable => s_fifo_enable(sem_index),
                full => s_fifo_full(sem_index),
                empty => s_fifo_empty(sem_index),
                data_in => s_fifo_data_in(sem_index),
                data_out => s_fifo_data_out(sem_index)
            );
    end generate fifo_array;
    
    -- Concurrent Statements
    s_addr <= p_addr;
    s_data_in <= p_data_in;
    --p_data_out <= s_data_out;
    p_status <= s_resume & s_block & s_full & s_error & s_done;

    -- This process searchs for an free semaphore id.
    search_free : process (p_clk, p_rst)
        --variable row : integer range C_MAX_SEMAPHORES - 1 downto 0 := 0;
        --variable full : boolean;
        variable v_or_vector : std_logic_vector(0 to C_MAX_SEMAPHORES - 1);
    begin
        if p_rst = '0' then
            s_full <= '0';
            --s_free <= 0;
            --row := 0;
            --full := false;
        elsif rising_edge(p_clk) then
            
            v_or_vector(0) := '0' or (not s_bitmap(0));
            
            for row in 1 to C_MAX_SEMAPHORES - 1 loop
                v_or_vector(row) := v_or_vector(row - 1) or (not s_bitmap(row));
            end loop;
            
            s_full <= not v_or_vector(C_MAX_SEMAPHORES - 1);
            
            for row in C_MAX_SEMAPHORES - 1 downto 0 loop
                if (s_bitmap(row) = '0') then
                    s_free <= row;
                end if;
            end loop;
            --row := 0;
            --while (s_bitmap(row) = '1' and row < C_MAX_SEMAPHORES - 1) loop
            --    s_free <= row;
            --    row := row + 1;
            --end loop;
            --full := s_bitmap(row) = '1';
            --if full then
            --    s_full <= '1';
            --else
            --    s_full <= '0';
            --end if;
        end if;
    end process;

    -- Semaphore Process
    mainProc: process (p_clk, p_rst, p_cmd) is
              variable v_idx: integer range C_MAX_SEMAPHORES - 1 downto 0 := 0;
    begin
        -- Reset Handling
        if p_rst = '0' then
            s_bitmap <= (others => '0');
            s_idx <= 0;
            s_thread <= (others => '0');
            p_data_out <= (others => '0');
            s_fifo_rd_wr <= (others => '0');
            s_fifo_enable <= (others => '1');
            s_error <= '0';
            s_done <= '0';
            s_block <= '0';
            s_resume <= '0';
            s_state <= idle;
        elsif rising_edge(p_clk) then
            -- Finite State Machine Starts Here!
            case s_state is
                when idle =>
                    -- Always disable all fifos when idle ;
                    s_fifo_enable <= (others => '1');
                    case p_cmd is
                        when C_CMD_CREATE =>
                            p_data_out <= (others => '0');
                            s_error <= '0';
                            s_done <= '0';
                            s_block <= '0';
                            s_resume <= '0';
                            if (s_full = '0') then
                                s_bitmap(s_free) <= '1';
                                s_idx <= s_free;
                                s_value <= conv_std_logic_vector(conv_integer(s_data_in), C_VALUE_WIDTH);
                                s_state <= createCmd;
                            else
                                s_error <= '1';
                            end if;
                        
                        when C_CMD_DESTROY =>
                            p_data_out <= (others => '0');
                            s_error <= '0';
                            s_done <= '0';
                            s_block <= '0';
                            s_resume <= '0';
                            s_idx <= conv_integer(s_addr);
                            s_state <= destroyCmd;
                        
                        when C_CMD_UP =>
                            p_data_out <= (others => '0');
                            s_error <= '0';
                            s_done <= '0';
                            s_block <= '0';
                            s_resume <= '0';
                            v_idx := conv_integer(s_addr);
                            s_idx <= v_idx;
                            ---- Forward POP FIFO if
                            if (s_memory(v_idx)(C_VALUE_WIDTH - 1) = '1') then
                                s_fifo_rd_wr(v_idx)  <= '1';
                                s_fifo_enable(v_idx) <= '0';
                            end if;
                            s_state <= upCmd;

                        when C_CMD_DOWN =>
                            p_data_out <= (others => '0');
                            s_error <= '0';
                            s_done <= '0';
                            s_block <= '0';
                            s_resume <= '0';
                            s_idx <= conv_integer(s_addr);
                            s_thread <= s_data_in;
                            s_state <= downCmd;
                        
                        when others =>
                            null;
                    end case;

                when createCmd =>
                    s_memory(s_idx) <= s_value;
                    p_data_out <= conv_std_logic_vector(s_idx, C_FIFO_WIDTH);
                    s_done <= '1';
                    s_state <= idle;

                when destroyCmd =>
                    if (s_fifo_empty(s_idx) = '1') then
                        s_bitmap(s_idx) <= '0';
                        s_done <= '1';
                    else
                        s_error <= '1';
                    end if;
                    s_state <= idle;

                when downCmd =>
                    if (s_bitmap(s_idx) = '1') then
                        s_memory(s_idx) <= s_memory(s_idx) - 1;
                        if (SIGNED(s_memory(s_idx)) < 1) then
                            s_block <= '1';
                            if (s_fifo_full(s_idx) = '0') then
                                s_fifo_enable(s_idx) <= '0';
                                s_fifo_rd_wr(s_idx) <= '0';
                                s_fifo_data_in(s_idx) <= s_thread;
                            else
                                s_error <= '1';
                            end if;
                        end if;
                        s_done <= '1';
                    else
                        s_error <= '1';
                    end if;
                    s_state <= idle;

                when upCmd =>
                if (s_bitmap(s_idx) = '1') then
                    s_memory(s_idx) <= s_memory(s_idx) + 1;
                    if (s_memory(s_idx)(C_VALUE_WIDTH - 1) = '1') then
                        s_fifo_enable(s_idx) <= '1';
                        s_state <= resumeThr;
                    else
                        s_done <= '1';
                        s_state <= idle;
                    end if;
                else
                    s_error <= '1';
                    s_state <= idle;
                end if;
                
                when resumeThr =>
                    s_resume <= '1';
                    s_done <= '1';
                    p_data_out <= s_fifo_data_out(s_idx);
                    s_state <= idle;
            end case;
        end if;
    end process;
end Behavioral;