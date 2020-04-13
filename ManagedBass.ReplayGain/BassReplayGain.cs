using System.Runtime.InteropServices;

namespace ManagedBass.ReplayGain
{
    public class BassReplayGain
    {
        public const int BATCH_SLOTS = 256;

        const string DllName = "bass_replay_gain";

        [DllImport(DllName)]
        static extern bool BASS_REPLAY_GAIN_Init();

        /// <summary>
        /// Initialize.
        /// </summary>
        /// <returns></returns>
        public static bool Init()
        {
            return BASS_REPLAY_GAIN_Init();
        }

        [DllImport(DllName)]
        static extern bool BASS_REPLAY_GAIN_Process(int Handle, [Out] out ReplayGainInfo result);

        public static bool Process(int Handle, out ReplayGainInfo info)
        {
            return BASS_REPLAY_GAIN_Process(Handle, out info);
        }

        [DllImport(DllName)]
        static extern bool BASS_REPLAY_GAIN_ProcessBatch(int[] Handles, [Out] out ReplayGainBatchInfo result);

        public static bool ProcessBatch(int[] Handles, out ReplayGainBatchInfo info)
        {
            return BASS_REPLAY_GAIN_ProcessBatch(Handles, out info);
        }

        [DllImport(DllName)]
        static extern bool BASS_REPLAY_GAIN_Free();

        /// <summary>
        /// Free.
        /// </summary>
        /// <returns></returns>
        public static bool Free()
        {
            return BASS_REPLAY_GAIN_Free();
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct ReplayGainInfo
    {
        public int handle;
        public float peak;
        public float gain;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct ReplayGainBatchInfo
    {
        public float peak;
        public float gain;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = BassReplayGain.BATCH_SLOTS)]
        public ReplayGainInfo[] items;
    }
}
