# 
# Synthesis run script generated by Vivado
# 

proc create_report { reportName command } {
  set status "."
  append status $reportName ".fail"
  if { [file exists $status] } {
    eval file delete [glob $status]
  }
  send_msg_id runtcl-4 info "Executing : $command"
  set retval [eval catch { $command } msg]
  if { $retval != 0 } {
    set fp [open $status w]
    close $fp
    send_msg_id runtcl-5 warning "$msg"
  }
}
create_project -in_memory -part xc7a100tcsg324-1

set_param project.singleFileAddWarning.threshold 0
set_param project.compositeFile.enableAutoGeneration 0
set_param synth.vivado.isSynthRun true
set_msg_config -source 4 -id {IP_Flow 19-2162} -severity warning -new_severity info
set_property webtalk.parent_dir E:/new/class/MiniMIPS32/MiniMIPS32.cache/wt [current_project]
set_property parent.project_path E:/new/class/MiniMIPS32/MiniMIPS32.xpr [current_project]
set_property XPM_LIBRARIES {XPM_CDC XPM_MEMORY} [current_project]
set_property default_lib xil_defaultlib [current_project]
set_property target_language Verilog [current_project]
set_property ip_output_repo e:/new/class/MiniMIPS32/MiniMIPS32.cache/ip [current_project]
set_property ip_cache_permissions {read write} [current_project]
add_files e:/new/class/MiniMIPS32/and_inst.coe
read_verilog -library xil_defaultlib {
  E:/new/class/MiniMIPS32/MiniMIPS32.srcs/sources_1/new/defines.v
  E:/new/class/MiniMIPS32/MiniMIPS32.srcs/sources_1/new/MiniMIPS32.v
  E:/new/class/MiniMIPS32/MiniMIPS32.srcs/sources_1/new/exe_stage.v
  E:/new/class/MiniMIPS32/MiniMIPS32.srcs/sources_1/new/exemem_reg.v
  E:/new/class/MiniMIPS32/MiniMIPS32.srcs/sources_1/new/id_stage.v
  E:/new/class/MiniMIPS32/MiniMIPS32.srcs/sources_1/new/idexe_reg.v
  E:/new/class/MiniMIPS32/MiniMIPS32.srcs/sources_1/new/if_stage.v
  E:/new/class/MiniMIPS32/MiniMIPS32.srcs/sources_1/new/ifid_reg.v
  E:/new/class/MiniMIPS32/MiniMIPS32.srcs/sources_1/new/mem_stage.v
  E:/new/class/MiniMIPS32/MiniMIPS32.srcs/sources_1/new/memwb_reg.v
  E:/new/class/MiniMIPS32/MiniMIPS32.srcs/sources_1/new/regfile.v
  E:/new/class/MiniMIPS32/MiniMIPS32.srcs/sources_1/new/wb_stage.v
  E:/new/class/MiniMIPS32/MiniMIPS32.srcs/sources_1/new/MiniMIPS32_SYS.v
}
read_ip -quiet e:/new/class/MiniMIPS32/MiniMIPS32.srcs/sources_1/ip/clk_wiz_0/clk_wiz_0.xci
set_property used_in_implementation false [get_files -all e:/new/class/MiniMIPS32/MiniMIPS32.srcs/sources_1/ip/clk_wiz_0/clk_wiz_0_board.xdc]
set_property used_in_implementation false [get_files -all e:/new/class/MiniMIPS32/MiniMIPS32.srcs/sources_1/ip/clk_wiz_0/clk_wiz_0.xdc]
set_property used_in_implementation false [get_files -all e:/new/class/MiniMIPS32/MiniMIPS32.srcs/sources_1/ip/clk_wiz_0/clk_wiz_0_ooc.xdc]

read_ip -quiet e:/new/class/MiniMIPS32/MiniMIPS32.srcs/sources_1/ip/inst_rom/inst_rom.xci
set_property used_in_implementation false [get_files -all e:/new/class/MiniMIPS32/MiniMIPS32.srcs/sources_1/ip/inst_rom/inst_rom_ooc.xdc]

# Mark all dcp files as not used in implementation to prevent them from being
# stitched into the results of this synthesis run. Any black boxes in the
# design are intentionally left as such for best results. Dcp files will be
# stitched into the design at a later time, either when this synthesis run is
# opened, or when it is stitched into a dependent implementation run.
foreach dcp [get_files -quiet -all -filter file_type=="Design\ Checkpoint"] {
  set_property used_in_implementation false $dcp
}

synth_design -top MiniMIPS32_SYS -part xc7a100tcsg324-1


# disable binary constraint mode for synth run checkpoints
set_param constraints.enableBinaryConstraints false
write_checkpoint -force -noxdef MiniMIPS32_SYS.dcp
create_report "synth_1_synth_report_utilization_0" "report_utilization -file MiniMIPS32_SYS_utilization_synth.rpt -pb MiniMIPS32_SYS_utilization_synth.pb"
