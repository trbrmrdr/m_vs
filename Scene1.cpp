#include "Scene1.h"
#include "Helper.h"
#include "soundVal.h"

#define OPTION_TEXTURE "option.png"

using namespace App;

const int POSTFxID = 11;
const char* EffectFileTable[] = {
	"scene1.glsl", // 0
	"scene2.glsl",
	"scene3.glsl",
	"scene4.glsl",
	"scene5.glsl",
	"scene6.glsl",
	"scene7.glsl",
	"scene8.glsl",
	"scene9.glsl",
	"scene10.glsl", // 9
	"scene11.glsl",
	"effect.glsl", // 11
	"scene21.glsl",
	"scene22.glsl"
};

Scene1::Scene1():
	width(0), height(0),
	nowEffect(0),
	editMode(false),
	nowCompiled(false),
	saveFronBuf(false),
	FPS(0),
	baseTime(-1),
	sound_system("Anuch-Metro.mp3") {}

Scene1::~Scene1() {
	free();
}



long getHash(const char* fileName) {
	FILE* fp = fopen(fileName, "rt");
	if (fp == NULL)
		return -1;
	long ret = Helper::GetLastDataEdit(fileName);
	fclose(fp);
	return ret;

}
void Scene1::initHashFile() {
	hashEffectFileTable.clear();
	for (int i = 0; i < EffectNum; ++i)
	{
		const char* file = EffectFileTable[i];
		long hash = getHash(file);
		if (hash != -1)
			hashEffectFileTable.insert(std::pair<std::string, long>(file, hash));
	}
}

void Scene1::checkValidHashFile() {
	const char* file = EffectFileTable[nowEffect];
	std::map<std::string, long>::iterator it = hashEffectFileTable.find(std::string(file));
	if (it != hashEffectFileTable.end())
	{
		long newD = Helper::GetLastDataEdit(file);
		if (newD != it->second)
		{
			it->second = newD;
			loadEffect(nowEffect);
			return;
		}
	}
	else
	{
		long hash = getHash(file);
		if (hash != -1)
			hashEffectFileTable.insert(std::pair<std::string, long>(file, hash));
	}
}

int Scene1::init(int width_, int height_) {

	width = width_;
	height = height_;

	initHashFile();
	//Sound
	sound_system.play_music();
	if (NULL == audio_spectr_l)
	{
		audio_spectr_l = new float[SPECTRUMSIZE];
		audio_spectr_r = new float[SPECTRUMSIZE];
	}

	//glew
	if (glewInit() != GLEW_OK)
	{
		Logger::Instance()->OutputString("Error: glewInit()");
		return -1;
	}
	glViewport(0, 0, width, height);

	//Textures

	optionTexture = Helper::LoadTexture(OPTION_TEXTURE);

	// Initialize back buffer
	backTexture = 0;
	glGenTextures(1, &backTexture);
	glBindTexture(GL_TEXTURE_2D, backTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Initialize frame buffer
	renderTexture = 0;
	glGenTextures(1, &renderTexture);
	glBindTexture(GL_TEXTURE_2D, renderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	frameBuffer = 0;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);

	// Initialize render buffer
	renderBuffer = 0;
	glGenRenderbuffers(1, &renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

	// Initialize render texture
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	errorHighlight = true;

	BitmapFontGL::Instance()->CreateTexture();
	glGenTextures(1, &audioTexture);


	shaderGL[POSTFxID].CompileFromFile(EffectFileTable[POSTFxID]);
	textEditor.Load(EffectFileTable[nowEffect]);

	//baseTime = -1;
	nowSource.clear();
	errorHighlight = true;
	return 0;
}

void Scene1::free() {

	BitmapFontGL::Instance()->Free();

	if (audioTexture != 0)
		glDeleteTextures(1, &audioTexture);

	for (int i = 0; i < EffectNum; ++i)
		shaderGL[i].Free();
	//delete audioAnalyzer;
	if (NULL != audio_spectr_l)
		delete audio_spectr_l; audio_spectr_l = NULL;
	if (NULL != audio_spectr_r)
		delete audio_spectr_r; audio_spectr_r = NULL;


	if (frameBuffer != 0)
		glDeleteFramebuffers(1, &frameBuffer);
	if (renderBuffer != 0)
		glDeleteRenderbuffers(1, &renderBuffer);
	if (renderTexture != 0)
		glDeleteTextures(1, &renderTexture);
	if (backTexture != 0)
		glDeleteTextures(1, &backTexture);
	if (optionTexture != 0)
		glDeleteTextures(1, &optionTexture);

	audioTexture = 0;
	frameBuffer = 0;
	renderBuffer = 0;
	renderTexture = 0;
	backTexture = 0;
	optionTexture = 0;
}

void Scene1::changeMouse(int mouseX, int mouseY) {
	mouseBuffer.SetPosition(( (float) mouseX / width ), 1.0f - (float) mouseY / height);
}

int Scene1::changeKeys(SDL_Keysym keysym) {
	keyBuffer.Clear();

	//Uint16 ch = keysym.scancode;
	//SDLKey key = isprint(ch)?(SDLKey)ch:keysym.sym;

	if (editMode)
		keyAnalyzer.KeyHit(&textEditor, keysym, EffectFileTable[nowEffect]);
	if (keysym.sym == SDLK_ESCAPE)
		return -1;

	int ctrl = keysym.mod & KMOD_CTRL;
	int alt = keysym.mod & KMOD_ALT;

	if (SDLK_F1 <= keysym.sym && keysym.sym <= SDLK_F12 && keysym.sym != SDLK_F11)
	{
		int nowEff = keysym.sym - SDLK_F1;
		if (alt)
			nowEff += 12;
		else if (ctrl)
			nowEff += 24;

		if (nowEffect != nowEff)
		{
			nowEffect = nowEff;
			loadEffect(nowEffect);
		}
		return 1;
	}

	if (keysym.sym == SDLK_F11)
	{
		if (editMode)
			editMode = false;
		else
			editMode = true;
		return 1;
	}

	if (keysym.mod & KMOD_CTRL && keysym.sym == SDLK_TAB)
	{
		errorHighlight = !errorHighlight;
		return 1;
	}

	if (keysym.sym == SDLK_q)
	{
		saveFronBuf = true;
		return 1;
	}
	return 0;
}

int prev = -1;
void Scene1::loadEffect(int effectId) {
	std::cout << "curr Effect = " << effectId << std::endl;
	if (prev != -1)
		shaderGL[prev].Unbind();
	prev = effectId;

	shaderGL[effectId].CompileFromFile(EffectFileTable[effectId]);
	textEditor.Load(EffectFileTable[effectId]);
}

int Scene1::Update(Uint32 nowTime) {
	if (-1 == baseTime)
		baseTime = nowTime;
	float realSec = ( nowTime - baseTime ) / 1000.0f;
	float delay = ( nowTime - prevTime ) / 1000.0f;
	FPS++;
	if (delay >= .5f)
	{
		prevTime = nowTime;
		fps = (int) ( (float) FPS / delay );
		FPS = 0;
	}
	//prevTime = nowTime;
	checkValidHashFile();

	sound_system.update();
	sound_system.get_spectrum(audio_spectr_l, audio_spectr_r);

	render(realSec);
	return 0;
}

void Scene1::render(float realSec) {
	// Swap render target
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	std::string str = textEditor.ToString();
	if (str.empty())
		str = str;
	if (nowSource != str)
	{
		nowSource = str;

		if (shaderGL[nowEffect].Compile(nowSource) != 0)
			nowCompiled = true;
		else
			nowCompiled = false;
	}

	float low = 0.0f;
	float mid = 0.0f;
	float high = 0.0f;
	float cy = -9999.0f;

	const int fontWidth = 10;
	const int fontHeight = 15;
	BitmapFontGL::Instance()->SetFontSize(fontWidth, fontHeight);




	if (shaderGL[nowEffect].Valid())
	{
		shaderGL[nowEffect].Bind();
		shaderGL[nowEffect].SetUniform("resolution", (float) width, (float) height);
		shaderGL[nowEffect].SetUniform("time", realSec);
		shaderGL[nowEffect].SetUniform("mouse", mouseBuffer.GetCursorX(), mouseBuffer.GetCursorY());

		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, optionTexture);
		shaderGL[nowEffect].SetUniform("optTex", (int) 1);//1

		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, backTexture);
		shaderGL[nowEffect].SetUniform("backbuffer", (int) 2);//2


		// Calculate low, mid, high freq.
		if (audio_spectr_l != NULL)
			//if (false)
		{
			if (lastSec == -1)
				lastSec = realSec;
			isAudioCalc = ( realSec - lastSec ) >= delAudioCalc;

			//if (isAudioCalc)
			{
				lastSec = realSec;

				const unsigned int image_width = SPECTRUMSIZE;
				const unsigned int image_height = 2;
				int scanLineWidh = ( ( 3 * image_width ) % 4 == 0 ) ? 3 * image_width : ( ( 3 * image_width ) / 4 ) * 4 + 4;

				GLfloat* texture = (GLfloat*) calloc(image_height*scanLineWidh, sizeof(GLfloat));

				for (int x = 0; x < image_width; x++)
					for (int y = 0; y < image_height; y++)
					{
						float left = audio_spectr_l[x];
						float right = audio_spectr_r[x];
						texture[( y*scanLineWidh + 3 * x )] = left;
						texture[( y*scanLineWidh + 3 * x ) + 1] = right;
						//texture[(y*scanLineWidh + 3 * x) + 2] = var;
					}

				/*
				//GLfloat texture[1024];

				glEnable(GL_TEXTURE_1D);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_1D, audioTexture);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				CheckGLError();
				glTexImage1D(GL_TEXTURE_1D, 0, GL_RED, image_width, 0, GL_RED, GL_FLOAT, texture);
				CheckGLError();
				*/
				glEnable(GL_TEXTURE_2D);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, audioTexture);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_FLOAT, texture);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				CHECK_GL_ERROR();
				shaderGL[nowEffect].SetUniform("fft", (int) 0);

				delete texture;

				float* audioBuffer = audio_spectr_l;

				int lowband = (int) floor(500.0 * (float) SPECTRUMSIZE / 44100.0 + 0.5);
				int midband = (int) floor(5000.0 * (float) SPECTRUMSIZE / 44100.0 + 0.5);

				for (int i = 0; i < lowband; i++)
					low += audioBuffer[i];
				for (int i = lowband; i < midband; i++)
					mid += audioBuffer[i];
				for (int i = midband; i < SPECTRUMSIZE; i++)
					high += audioBuffer[i];

				//- 500 low
				// 500 - 5000 mid
				// 5000 - high

				low /= lowband;
				mid /= ( midband - lowband );
				high /= ( SPECTRUMSIZE - midband );


				/*
				shaderGL[nowEffect].SetUniform("lowFreq", (float) low);
				shaderGL[nowEffect].SetUniform("midFreq", (float) mid);
				shaderGL[nowEffect].SetUniform("highFreq", (float) high);
				*/

				mLow.setVar(low, realSec);
				mMid.setVar(mid, realSec);
				mHigh.setVar(high, realSec);


				float randVal = ( (float) rand() / (float) RAND_MAX )*.5;
				mX.setVar(randVal, realSec);
				mY.setVar(randVal, realSec);
			}

			{
				float low = mLow.getp(realSec);
				float mid = mMid.getp(realSec);
				float high = mHigh.getp(realSec);
				shaderGL[nowEffect].SetUniform("lowFreq", (float) low);
				shaderGL[nowEffect].SetUniform("midFreq", (float) mid);
				shaderGL[nowEffect].SetUniform("highFreq", (float) high);


				float tX = mX.getp(realSec);
				float tY = mY.getp(realSec);
				shaderGL[nowEffect].SetUniform("val_1", tX, tY);
			}
		}



		//#####
		if (true)
		{
			glPushMatrix();
			glPushAttrib(GL_ENABLE_BIT);

			glLineWidth(30);
			glBegin(GL_LINES);

			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(1.f, 1.f);

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glVertex2f(1.0f, 0.1f);
			glVertex2f(.5f, .5f);

			glEnd();

			glPopAttrib();
			glPopMatrix();
		}
		//#####
		glRecti(1, 1, -1, -1);
		shaderGL[nowEffect].Unbind();

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTexture(GL_TEXTURE0);
	}

	// Copy render texture to back buffer texture
	glBindTexture(GL_TEXTURE_2D, backTexture);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, width, height, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (saveFronBuf)
	{
		Helper::SaveLastTexture("test.png", width, height);
		saveFronBuf = false;
	}

	if (editMode)
	{
		//keyAnalyzer.Input(&textEditor, EffectFileTable[nowEffect]);
		glPushMatrix();
		// TextEditor Background
		const float aspect = width / static_cast<float>( height );
		const float textEditorHeight = textEditor.GetMaxLineNum() * fontHeight * ( 0.25f * 8.0f / width * aspect );
		const float textEditorBGHeight = textEditorHeight * 1.2f;
		const float editorOffsetY = -( 2.0f - textEditorHeight ) / 2.0f;
		const float editorBGOffsetY = -( 2.0f - textEditorBGHeight ) / 2.0f;
		glPushMatrix();
		glTranslatef(-1.0f, 1.0f + editorBGOffsetY, 0.f);
		glPushAttrib(GL_ENABLE_BIT);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// render editor background
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBegin(GL_QUADS);
		glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(0.0f + width, 0.0f);
		glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
		glVertex2f(0.0f + width, -textEditorBGHeight / 5.0f);
		glVertex2f(0.0f, -textEditorBGHeight / 5.0f);
		glEnd();
		glBegin(GL_QUADS);
		glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
		glVertex2f(0.0f, -textEditorBGHeight / 5.0f);
		glVertex2f(0.0f + width, -textEditorBGHeight / 5.0f);
		glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
		glVertex2f(0.0f + width, -4.0f*textEditorBGHeight / 5.0f);
		glVertex2f(0.0f, -4.0f*textEditorBGHeight / 5.0f);
		glEnd();
		glBegin(GL_QUADS);
		glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
		glVertex2f(0.0f, -4.0f*textEditorBGHeight / 5.0f);
		glVertex2f(0.0f + width, -4.0f*textEditorBGHeight / 5.0f);
		glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
		glVertex2f(0.0f + width, -textEditorBGHeight);
		glVertex2f(0.0f, -textEditorBGHeight);
		glEnd();
		glPopAttrib();
		glPopMatrix();


		// render editor
		glTranslatef(-1.0f, 1.0f + editorOffsetY, 0.f);
		TextEditorPtrBuffer ptrbuf = textEditor.GetVisibleText();
		TextEditorPtrBuffer textbuf = textEditor.GetText();

		bool upAlpha = true;
		bool downAlpha = true;

		if (textEditor.GetLineOffset() == 0)
		{
			upAlpha = false;
		}

		if (textEditor.GetLineOffset() + textEditor.GetMaxLineNum() >= textEditor.GetLineNum())
		{
			downAlpha = false;
		}

		const int transRange = 5;
		BitmapFontGL::Instance()->Reset();
		for (int i = 0; i < textEditor.GetLineOffset(); i++)
		{
			BitmapFontGL::Instance()->ProcessComment(*textbuf[i]);
		}

		std::set<int> errorLinesFS = shaderGL[nowEffect].GetErrorLinesFS();

		for (int i = 0; i < ptrbuf.size(); i++)
		{
			float up = 1.0f;
			float down = 1.0f;

			if (upAlpha)
			{
				if (i < transRange)
				{
					up = (float) i / transRange;
					down = (float) ( i + 1 ) / transRange;
				}
			}
			if (downAlpha)
			{
				if (i >= ptrbuf.size() - transRange)
				{
					up = 1.0f - (float) ( i - ptrbuf.size() + transRange ) / transRange;
					down = 1.0f - (float) ( i + 1 - ptrbuf.size() + transRange ) / transRange;
				}
			}

			bool errorLine = false;
			if (errorLinesFS.find(i + textEditor.GetLineOffset() + 1) != errorLinesFS.end())
				errorLine = true;
			if (errorHighlight == false)
				errorLine = false;

			BitmapFontGL::Instance()->DrawLine(ptrbuf[i]->c_str(), aspect, width, i, up, down, errorLine);
			if (i == textEditor.GetCursorPosition().col)
				cy = height - i * fontHeight - fontHeight / 2.0f + height * editorOffsetY / 2.0f;
		}


		if (nowCompiled)
			BitmapFontGL::Instance()->DrawLine(EffectFileTable[nowEffect], aspect, width, textEditor.GetMaxLineNum(), 0.5f, 0.5f);
		else
			BitmapFontGL::Instance()->DrawLine(EffectFileTable[nowEffect], aspect, width, textEditor.GetMaxLineNum(), 0.5f, 0.5f, 1.0f, 0.0f, 0.0f);

		BitmapFontGL::Instance()->DrawLine("F1-F10: Change File  F11: Show/Hide code  F12: Edit PostFx", aspect, width, textEditor.GetMaxLineNum() + 1, 0.5f, 0.5f);
		char tmp[100];
		sprintf(tmp, "realSec = %f   FPS = %i", realSec, fps);
		BitmapFontGL::Instance()->DrawLine(tmp, aspect, width, textEditor.GetMaxLineNum() + 2, 0.5f, 0.5f);


		// render editor cursor
		EditorCursor cursor = textEditor.GetCursorPosition();
		BitmapFontGL::Instance()->DrawCursor(cursor.col, cursor.row, aspect, width);

		if (textEditor.IsSelectMode())
		{
			EditorCursor selectStart = textEditor.GetSelectStart();
			if (selectStart.col > cursor.col)
				std::swap(selectStart, cursor);

			if (selectStart.col == cursor.col)
			{
				int start = selectStart.row;
				int end = cursor.row;


				BitmapFontGL::Instance()->DrawSelect(aspect, width, selectStart.col, start, end, 0.5f, 0.5f, 0.5f);
			}
			else
			{
				for (int i = selectStart.col; i <= cursor.col; i++)
				{
					if (i < 0 || i >= textEditor.GetMaxLineNum())
						continue;

					if (i == selectStart.col)
					{
						BitmapFontGL::Instance()->DrawSelect(aspect, width, i, selectStart.row, textEditor.GetLineLength(i));
					}
					else if (i < cursor.col)
					{
						BitmapFontGL::Instance()->DrawSelect(aspect, width, i, 0, textEditor.GetLineLength(i));
					}
					else if (i == cursor.col)
					{
						BitmapFontGL::Instance()->DrawSelect(aspect, width, i, 0, cursor.row);
					}
				}
			}
		}
		//		BitmapFontGL::Instance()->DrawSelect(aspect, width, 5, 1, 9);

		glPopMatrix();
	}



	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (shaderGL[POSTFxID].Valid())
	{
		shaderGL[POSTFxID].Bind();

		glBindTexture(GL_TEXTURE_2D, renderTexture);
		shaderGL[POSTFxID].SetUniform("texture0", (int) 0);
		shaderGL[POSTFxID].SetUniform("resolution", (float) width, (float) height);
		shaderGL[POSTFxID].SetUniform("time", realSec);
		shaderGL[POSTFxID].SetUniform("mouse", mouseBuffer.GetCursorX(), mouseBuffer.GetCursorY());
		shaderGL[POSTFxID].SetUniform("lowFreq", (float) low);
		shaderGL[POSTFxID].SetUniform("midFreq", (float) mid);
		shaderGL[POSTFxID].SetUniform("highFreq", (float) high);
		shaderGL[POSTFxID].SetUniform("editorCursorY", (float) cy);

		//float cy = (textEditor.GetCursorPosition().col - textEditor.GetLineOffset()) * 11

		glRecti(1, 1, -1, -1);
		glBindTexture(GL_TEXTURE_2D, 0);

		shaderGL[POSTFxID].Unbind();
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, renderTexture);
		glRecti(1, 1, -1, -1);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
