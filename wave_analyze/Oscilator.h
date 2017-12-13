#ifndef YR_WAVEANALYZE_OSCILATOR_H_
#define YR_WAVEANALYZE_OSCILATOR_H_

#include <cinttypes>

namespace yr {
namespace waveanalyze {

class Oscilator {
public:
	/**
	 * コンストラクタ。いったん設定したパラメータは変更不可。
	 */
	Oscilator(
		const uint32_t sampleing_rate,///< [in] サンプリング周波数
		const double wb,			///< [in] 共振周波数
		const double zeta			///< [in] 減衰定数
		);

	/**
	 * デストラクタ。ほぼ何もしない。
	 */
	~Oscilator();

	/**
	 * 外力を入力する
	 */
	void InputExternalForce(
		const double force			///< [in] 外力
		);

	/**
	 * 現在位置を取得する
	 * 
	 * @retval 現在位置
	 */
	double GetPos();

	/**
	 * 現在振幅を取得する
	 *
	 * @retval 現在振幅
	 */
	double GetAmplitude();

	/**
	 * 現在時刻を取得する
	 *
	 * @retval 現在時刻
	 */
	uint64_t GetTime();

	/**
	 * いままでの計算結果をクリアする
	 */
	void Clear();

private:
	//サンプリング周波数
	const uint32_t sampling_rate_;
	//周波数
	const double   wn_;
	//減衰定数
	const double   zeta_;

	//位置
	double pos_;
	double pos_diff_;
	//速度
	double vel_;
	double vel_diff_;

	//振幅 (現在振幅)
	double amplitude_;
	//位置が増加中かどうか
	bool is_pos_increasing_;

	//現在時刻
	uint64_t time_;
};

}; //namespace waveanalyze
}; //namespace yr

#endif //YR_WAVE_OSCILATOR_H_
