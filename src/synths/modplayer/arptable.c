#include "config.h"

#ifdef MODPLAYER_ENABLED

const float finetune[16] = {
		1.000000000f,
		0.992805720f,
		0.985663198f,
		0.978572062f,
		0.971531941f,
		0.964542468f,
		0.957603280f,
		0.950714015f,
		1.059463094f,
		1.051841020f,
		1.044273782f,
		1.036760985f,
		1.029302236f,
		1.021897148f,
		1.014545334f,
		1.007246412f
};

const float arptable[16] = { 1.000f, 1.059f, 1.122f, 1.189f, 1.259f, 1.334f,
		1.414f, 1.498f, 1.587f, 1.681f, 1.781f, 1.887f, 2.000f, 2.118f, 2.244f,
		2.378f };

const float arptable_reciproc[16] = { 1.000f, 0.944f, 0.891f, 0.841f, 0.794f,
		0.749f, 0.707f, 0.667f, 0.630f, 0.595f, 0.561f, 0.530f, 0.500f, 0.472f,
		0.445f, 0.420f };

const float vibratotable[16][64] = { { 1.0000f, 1.0000f, 1.0000f, 1.0000f,
		1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f,
		1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f,
		1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f,
		1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f,
		1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f,
		1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f,
		1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f, 1.0000f,
		1.0000f, 1.0000f, 1.0000f, 1.0000f },

		{ 1.0000f, 1.0004f, 1.0007f, 1.0010f, 1.0014f, 1.0017f, 1.0020f,
				1.0023f, 1.0026f, 1.0028f, 1.0030f, 1.0032f, 1.0033f, 1.0035f,
				1.0035f, 1.0036f, 1.0036f, 1.0036f, 1.0035f, 1.0035f, 1.0033f,
				1.0032f, 1.0030f, 1.0028f, 1.0026f, 1.0023f, 1.0020f, 1.0017f,
				1.0014f, 1.0010f, 1.0007f, 1.0004f, 1.0000f, 0.9996f, 0.9993f,
				0.9990f, 0.9986f, 0.9983f, 0.9980f, 0.9977f, 0.9975f, 0.9972f,
				0.9970f, 0.9968f, 0.9967f, 0.9966f, 0.9965f, 0.9964f, 0.9964f,
				0.9964f, 0.9965f, 0.9966f, 0.9967f, 0.9968f, 0.9970f, 0.9972f,
				0.9975f, 0.9977f, 0.9980f, 0.9983f, 0.9986f, 0.9990f, 0.9993f,
				0.9996f },

		{ 1.0000f, 1.0007f, 1.0014f, 1.0021f, 1.0028f, 1.0034f, 1.0040f,
				1.0046f, 1.0051f, 1.0056f, 1.0060f, 1.0064f, 1.0067f, 1.0069f,
				1.0071f, 1.0072f, 1.0072f, 1.0072f, 1.0071f, 1.0069f, 1.0067f,
				1.0064f, 1.0060f, 1.0056f, 1.0051f, 1.0046f, 1.0040f, 1.0034f,
				1.0028f, 1.0021f, 1.0014f, 1.0007f, 1.0000f, 0.9993f, 0.9986f,
				0.9979f, 0.9972f, 0.9966f, 0.9960f, 0.9954f, 0.9949f, 0.9944f,
				0.9940f, 0.9937f, 0.9934f, 0.9931f, 0.9929f, 0.9928f, 0.9928f,
				0.9928f, 0.9929f, 0.9931f, 0.9934f, 0.9937f, 0.9940f, 0.9944f,
				0.9949f, 0.9954f, 0.9960f, 0.9966f, 0.9972f, 0.9979f, 0.9986f,
				0.9993f },

		{ 1.0000f, 1.0011f, 1.0021f, 1.0031f, 1.0042f, 1.0051f, 1.0060f,
				1.0069f, 1.0077f, 1.0084f, 1.0090f, 1.0096f, 1.0101f, 1.0104f,
				1.0107f, 1.0108f, 1.0109f, 1.0108f, 1.0107f, 1.0104f, 1.0101f,
				1.0096f, 1.0090f, 1.0084f, 1.0077f, 1.0069f, 1.0060f, 1.0051f,
				1.0042f, 1.0031f, 1.0021f, 1.0011f, 1.0000f, 0.9989f, 0.9979f,
				0.9969f, 0.9959f, 0.9949f, 0.9940f, 0.9932f, 0.9924f, 0.9917f,
				0.9910f, 0.9905f, 0.9900f, 0.9897f, 0.9894f, 0.9893f, 0.9892f,
				0.9893f, 0.9894f, 0.9897f, 0.9900f, 0.9905f, 0.9910f, 0.9917f,
				0.9924f, 0.9932f, 0.9940f, 0.9949f, 0.9959f, 0.9969f, 0.9979f,
				0.9989f }, { 1.0000f, 1.0014f, 1.0028f, 1.0042f, 1.0055f,
				1.0068f, 1.0081f, 1.0092f, 1.0103f, 1.0112f, 1.0121f, 1.0128f,
				1.0134f, 1.0139f, 1.0143f, 1.0145f, 1.0145f, 1.0145f, 1.0143f,
				1.0139f, 1.0134f, 1.0128f, 1.0121f, 1.0112f, 1.0103f, 1.0092f,
				1.0081f, 1.0068f, 1.0055f, 1.0042f, 1.0028f, 1.0014f, 1.0000f,
				0.9986f, 0.9972f, 0.9958f, 0.9945f, 0.9932f, 0.9920f, 0.9909f,
				0.9898f, 0.9889f, 0.9881f, 0.9873f, 0.9867f, 0.9863f, 0.9859f,
				0.9857f, 0.9857f, 0.9857f, 0.9859f, 0.9863f, 0.9867f, 0.9873f,
				0.9881f, 0.9889f, 0.9898f, 0.9909f, 0.9920f, 0.9932f, 0.9945f,
				0.9958f, 0.9972f, 0.9986f }, { 1.0000f, 1.0018f, 1.0035f,
				1.0053f, 1.0069f, 1.0085f, 1.0101f, 1.0115f, 1.0128f, 1.0141f,
				1.0151f, 1.0160f, 1.0168f, 1.0174f, 1.0179f, 1.0181f, 1.0182f,
				1.0181f, 1.0179f, 1.0174f, 1.0168f, 1.0160f, 1.0151f, 1.0141f,
				1.0128f, 1.0115f, 1.0101f, 1.0085f, 1.0069f, 1.0053f, 1.0035f,
				1.0018f, 1.0000f, 0.9982f, 0.9965f, 0.9948f, 0.9931f, 0.9915f,
				0.9900f, 0.9886f, 0.9873f, 0.9861f, 0.9851f, 0.9842f, 0.9835f,
				0.9829f, 0.9825f, 0.9822f, 0.9821f, 0.9822f, 0.9825f, 0.9829f,
				0.9835f, 0.9842f, 0.9851f, 0.9861f, 0.9873f, 0.9886f, 0.9900f,
				0.9915f, 0.9931f, 0.9948f, 0.9965f, 0.9982f },
		{ 1.0000f, 1.0021f, 1.0042f, 1.0063f, 1.0083f, 1.0103f, 1.0121f,
				1.0138f, 1.0154f, 1.0169f, 1.0182f, 1.0193f, 1.0202f, 1.0209f,
				1.0215f, 1.0218f, 1.0219f, 1.0218f, 1.0215f, 1.0209f, 1.0202f,
				1.0193f, 1.0182f, 1.0169f, 1.0154f, 1.0138f, 1.0121f, 1.0103f,
				1.0083f, 1.0063f, 1.0042f, 1.0021f, 1.0000f, 0.9979f, 0.9958f,
				0.9937f, 0.9917f, 0.9898f, 0.9880f, 0.9864f, 0.9848f, 0.9834f,
				0.9822f, 0.9811f, 0.9802f, 0.9795f, 0.9790f, 0.9787f, 0.9786f,
				0.9787f, 0.9790f, 0.9795f, 0.9802f, 0.9811f, 0.9822f, 0.9834f,
				0.9848f, 0.9864f, 0.9880f, 0.9898f, 0.9917f, 0.9937f, 0.9958f,
				0.9979f }, { 1.0000f, 1.0025f, 1.0049f, 1.0074f, 1.0097f,
				1.0120f, 1.0141f, 1.0162f, 1.0180f, 1.0197f, 1.0212f, 1.0225f,
				1.0236f, 1.0245f, 1.0251f, 1.0255f, 1.0256f, 1.0255f, 1.0251f,
				1.0245f, 1.0236f, 1.0225f, 1.0212f, 1.0197f, 1.0180f, 1.0162f,
				1.0141f, 1.0120f, 1.0097f, 1.0074f, 1.0049f, 1.0025f, 1.0000f,
				0.9975f, 0.9951f, 0.9927f, 0.9904f, 0.9882f, 0.9861f, 0.9841f,
				0.9823f, 0.9807f, 0.9792f, 0.9780f, 0.9769f, 0.9761f, 0.9755f,
				0.9752f, 0.9750f, 0.9752f, 0.9755f, 0.9761f, 0.9769f, 0.9780f,
				0.9792f, 0.9807f, 0.9823f, 0.9841f, 0.9861f, 0.9882f, 0.9904f,
				0.9927f, 0.9951f, 0.9975f }, { 1.0000f, 1.0028f, 1.0057f,
				1.0084f, 1.0111f, 1.0137f, 1.0162f, 1.0185f, 1.0206f, 1.0226f,
				1.0243f, 1.0258f, 1.0270f, 1.0280f, 1.0287f, 1.0292f, 1.0293f,
				1.0292f, 1.0287f, 1.0280f, 1.0270f, 1.0258f, 1.0243f, 1.0226f,
				1.0206f, 1.0185f, 1.0162f, 1.0137f, 1.0111f, 1.0084f, 1.0057f,
				1.0028f, 1.0000f, 0.9972f, 0.9944f, 0.9917f, 0.9890f, 0.9865f,
				0.9841f, 0.9818f, 0.9798f, 0.9779f, 0.9763f, 0.9749f, 0.9737f,
				0.9727f, 0.9721f, 0.9717f, 0.9715f, 0.9717f, 0.9721f, 0.9727f,
				0.9737f, 0.9749f, 0.9763f, 0.9779f, 0.9798f, 0.9818f, 0.9841f,
				0.9865f, 0.9890f, 0.9917f, 0.9944f, 0.9972f },
		{ 1.0000f, 1.0032f, 1.0064f, 1.0095f, 1.0125f, 1.0154f, 1.0182f,
				1.0208f, 1.0232f, 1.0254f, 1.0274f, 1.0291f, 1.0305f, 1.0316f,
				1.0324f, 1.0329f, 1.0330f, 1.0329f, 1.0324f, 1.0316f, 1.0305f,
				1.0291f, 1.0274f, 1.0254f, 1.0232f, 1.0208f, 1.0182f, 1.0154f,
				1.0125f, 1.0095f, 1.0064f, 1.0032f, 1.0000f, 0.9968f, 0.9937f,
				0.9906f, 0.9876f, 0.9848f, 0.9821f, 0.9796f, 0.9773f, 0.9752f,
				0.9733f, 0.9718f, 0.9704f, 0.9694f, 0.9686f, 0.9682f, 0.9680f,
				0.9682f, 0.9686f, 0.9694f, 0.9704f, 0.9718f, 0.9733f, 0.9752f,
				0.9773f, 0.9796f, 0.9821f, 0.9848f, 0.9876f, 0.9906f, 0.9937f,
				0.9968f }, { 1.0000f, 1.0035f, 1.0071f, 1.0105f, 1.0139f,
				1.0172f, 1.0203f, 1.0232f, 1.0259f, 1.0283f, 1.0305f, 1.0324f,
				1.0339f, 1.0352f, 1.0360f, 1.0366f, 1.0368f, 1.0366f, 1.0360f,
				1.0352f, 1.0339f, 1.0324f, 1.0305f, 1.0283f, 1.0259f, 1.0232f,
				1.0203f, 1.0172f, 1.0139f, 1.0105f, 1.0071f, 1.0035f, 1.0000f,
				0.9965f, 0.9930f, 0.9896f, 0.9863f, 0.9831f, 0.9801f, 0.9774f,
				0.9748f, 0.9725f, 0.9704f, 0.9687f, 0.9672f, 0.9660f, 0.9652f,
				0.9647f, 0.9645f, 0.9647f, 0.9652f, 0.9660f, 0.9672f, 0.9687f,
				0.9704f, 0.9725f, 0.9748f, 0.9774f, 0.9801f, 0.9831f, 0.9863f,
				0.9896f, 0.9930f, 0.9965f }, { 1.0000f, 1.0039f, 1.0078f,
				1.0116f, 1.0153f, 1.0189f, 1.0223f, 1.0255f, 1.0285f, 1.0312f,
				1.0336f, 1.0356f, 1.0374f, 1.0387f, 1.0397f, 1.0403f, 1.0405f,
				1.0403f, 1.0397f, 1.0387f, 1.0374f, 1.0356f, 1.0336f, 1.0312f,
				1.0285f, 1.0255f, 1.0223f, 1.0189f, 1.0153f, 1.0116f, 1.0078f,
				1.0039f, 1.0000f, 0.9961f, 0.9923f, 0.9885f, 0.9849f, 0.9815f,
				0.9782f, 0.9751f, 0.9723f, 0.9698f, 0.9675f, 0.9656f, 0.9640f,
				0.9627f, 0.9618f, 0.9613f, 0.9611f, 0.9613f, 0.9618f, 0.9627f,
				0.9640f, 0.9656f, 0.9675f, 0.9698f, 0.9723f, 0.9751f, 0.9782f,
				0.9815f, 0.9849f, 0.9885f, 0.9923f, 0.9961f },
		{ 1.0000f, 1.0043f, 1.0085f, 1.0127f, 1.0167f, 1.0206f, 1.0244f,
				1.0279f, 1.0311f, 1.0341f, 1.0367f, 1.0389f, 1.0408f, 1.0423f,
				1.0434f, 1.0441f, 1.0443f, 1.0441f, 1.0434f, 1.0423f, 1.0408f,
				1.0389f, 1.0367f, 1.0341f, 1.0311f, 1.0279f, 1.0244f, 1.0206f,
				1.0167f, 1.0127f, 1.0085f, 1.0043f, 1.0000f, 0.9958f, 0.9916f,
				0.9875f, 0.9836f, 0.9798f, 0.9762f, 0.9729f, 0.9698f, 0.9671f,
				0.9646f, 0.9625f, 0.9608f, 0.9594f, 0.9584f, 0.9578f, 0.9576f,
				0.9578f, 0.9584f, 0.9594f, 0.9608f, 0.9625f, 0.9646f, 0.9671f,
				0.9698f, 0.9729f, 0.9762f, 0.9798f, 0.9836f, 0.9875f, 0.9916f,
				0.9958f }, { 1.0000f, 1.0046f, 1.0092f, 1.0137f, 1.0181f,
				1.0224f, 1.0264f, 1.0302f, 1.0337f, 1.0369f, 1.0398f, 1.0423f,
				1.0443f, 1.0459f, 1.0471f, 1.0478f, 1.0481f, 1.0478f, 1.0471f,
				1.0459f, 1.0443f, 1.0423f, 1.0398f, 1.0369f, 1.0337f, 1.0302f,
				1.0264f, 1.0224f, 1.0181f, 1.0137f, 1.0092f, 1.0046f, 1.0000f,
				0.9954f, 0.9909f, 0.9865f, 0.9822f, 0.9781f, 0.9743f, 0.9707f,
				0.9674f, 0.9644f, 0.9617f, 0.9595f, 0.9576f, 0.9561f, 0.9550f,
				0.9544f, 0.9542f, 0.9544f, 0.9550f, 0.9561f, 0.9576f, 0.9595f,
				0.9617f, 0.9644f, 0.9674f, 0.9707f, 0.9743f, 0.9781f, 0.9822f,
				0.9865f, 0.9909f, 0.9954f }, { 1.0000f, 1.0050f, 1.0099f,
				1.0148f, 1.0195f, 1.0241f, 1.0285f, 1.0326f, 1.0364f, 1.0398f,
				1.0429f, 1.0456f, 1.0478f, 1.0496f, 1.0508f, 1.0516f, 1.0518f,
				1.0516f, 1.0508f, 1.0496f, 1.0478f, 1.0456f, 1.0429f, 1.0398f,
				1.0364f, 1.0326f, 1.0285f, 1.0241f, 1.0195f, 1.0148f, 1.0099f,
				1.0050f, 1.0000f, 0.9951f, 0.9902f, 0.9854f, 0.9808f, 0.9765f,
				0.9723f, 0.9684f, 0.9649f, 0.9617f, 0.9588f, 0.9564f, 0.9544f,
				0.9528f, 0.9516f, 0.9509f, 0.9507f, 0.9509f, 0.9516f, 0.9528f,
				0.9544f, 0.9564f, 0.9588f, 0.9617f, 0.9649f, 0.9684f, 0.9723f,
				0.9765f, 0.9808f, 0.9854f, 0.9902f, 0.9951f }, { 1.0000f,
				1.0053f, 1.0106f, 1.0158f, 1.0209f, 1.0259f, 1.0305f, 1.0350f,
				1.0390f, 1.0427f, 1.0461f, 1.0489f, 1.0513f, 1.0532f, 1.0545f,
				1.0554f, 1.0556f, 1.0554f, 1.0545f, 1.0532f, 1.0513f, 1.0489f,
				1.0461f, 1.0427f, 1.0390f, 1.0350f, 1.0305f, 1.0259f, 1.0209f,
				1.0158f, 1.0106f, 1.0053f, 1.0000f, 0.9947f, 0.9895f, 0.9844f,
				0.9795f, 0.9748f, 0.9704f, 0.9662f, 0.9624f, 0.9590f, 0.9560f,
				0.9534f, 0.9512f, 0.9495f, 0.9483f, 0.9475f, 0.9473f, 0.9475f,
				0.9483f, 0.9495f, 0.9512f, 0.9534f, 0.9560f, 0.9590f, 0.9624f,
				0.9662f, 0.9704f, 0.9748f, 0.9795f, 0.9844f, 0.9895f, 0.9947f }

};
#endif
