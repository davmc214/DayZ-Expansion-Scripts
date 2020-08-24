/**
 * ExpansionHumanCommandLeavingVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionHumanCommandLeavingVehicle extends HumanCommandScript
{
	PlayerBase m_Player;
	ExpansionHumanST m_Table;
	HumanInputController m_Input;

	ExpansionVehicleScript m_ExVehicle;
	CarScript m_VnVehicle;
	Object m_GenVehicle;

	private float m_Time;
	private bool m_NeedFinish;

	private bool m_IsCar;
	private bool m_IsBoat;
	private bool m_IsHelicopter;
	private bool m_IsPlane;

	private float m_Speedometer;

	void ExpansionHumanCommandLeavingVehicle( PlayerBase player, Object veh, ExpansionHumanST table )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandLeavingVehicle::ExpansionHumanCommandLeavingVehicle Start");
		#endif

		m_Player = player;
		m_Input = m_Player.GetInputController();
		m_Table = table;

		m_GenVehicle = veh;

		if ( Class.CastTo( m_ExVehicle, m_GenVehicle ) )
		{
			m_IsCar = m_ExVehicle.IsCar();
			m_IsBoat = m_ExVehicle.IsBoat();
			m_IsHelicopter = m_ExVehicle.IsHelicopter();
			m_IsPlane = m_ExVehicle.IsPlane();
		} else if ( Class.CastTo( m_VnVehicle, m_GenVehicle ) )
		{
			m_IsCar = m_VnVehicle.IsCar();
			m_IsBoat = m_VnVehicle.IsBoat();
			m_IsHelicopter = m_VnVehicle.IsHelicopter();
			m_IsPlane = m_VnVehicle.IsPlane();

			m_Speedometer = m_VnVehicle.GetSpeedometer();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandLeavingVehicle::ExpansionHumanCommandLeavingVehicle End");
		#endif
	}

	void CrawlOutVehicle()
	{
		m_Table.CallVehicleCrawlOut( this );
	}

	void ClimbOutVehicle()
	{
		m_Table.CallVehicleClimbOut( this );
	}

	void JumpOutVehicle()
	{
		m_Table.CallVehicleJumpOut( this );
	}

	void GetOutVehicle()
	{
		m_Table.CallVehicleGetOut( this );
	}

	float GetSpeedometer()
	{
		return m_Speedometer;
	}

	bool IsCar()
	{
		return m_IsCar;
	}

	bool IsBoat()
	{
		return m_IsBoat;
	}

	bool IsHelicopter()
	{
		return m_IsHelicopter;
	}

	bool IsPlane()
	{
		return m_IsPlane;
	}

	override void OnActivate()
	{
		GetOutVehicle();

		m_Time = 0;
	}

	override void OnDeactivate()
	{
		if ( m_ExVehicle )
		{
			m_ExVehicle.CrewGetOut( m_ExVehicle.CrewMemberIndex( m_Player ) );
		
			if ( !m_ExVehicle.CanObjectAttach( m_Player ) )
				m_Player.UnlinkFromLocalSpace();
		} else if ( m_VnVehicle )
		{
			m_VnVehicle.CrewGetOut( m_VnVehicle.CrewMemberIndex( m_Player ) );
		
			if ( !m_VnVehicle.CanObjectAttach( m_Player ) )
				m_Player.UnlinkFromLocalSpace();
		}
		
		dBodyActive( m_Player, ActiveState.ALWAYS_ACTIVE );
		
		dBodyEnableGravity( m_Player, true );

		m_Player.OnCommandExpansionVehicleFinish();
	}

	override void PreAnimUpdate( float pDt )
	{
	}

	override void PrePhysUpdate( float pDt )
	{
		m_Time += pDt;

		if ( !m_NeedFinish && m_Time > 0.15 )
		{
			m_NeedFinish = true;
		}
	}

	override bool PostPhysUpdate( float pDt )
	{
		return m_NeedFinish == false;
	}
}
