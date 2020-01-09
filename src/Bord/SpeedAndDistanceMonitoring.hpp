#ifndef CCC
#define CCC

class SpeedAndDistanceMonitoring
{
	private :
		int V_MRSP = 130;
		int V_ebi;
		int V_sbi;
		int V_warning;
		std::string status = "CSM";
		std::string supervision_status = "Normal";// au démarrage il est en condition normal
		std::string command_triggered;
	public :
		void MSRP(TrackRelatedInputs TrackRI, TrainRelatedInputs TrainRI);
		void Supervision_limits(TrainRelatedInputs TrainRI);
		void SpeedAndDistanceMonitoringCommands(TrainRelatedInputs TrainRI);
		int getV_MRSP();
		int getV_ebi();
		int getV_sbi();
		int getV_warning();
		std::string getStatus();
		std::string getSupervision_status();
		std::string getCommand_triggered();
};



#endif