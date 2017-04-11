#include "stdafx.h"
#include "Core.h"
#include "soundVal.h"
#include "Scenes/Scenes.h"
Core::Core():
	m_editMode(false),
	m_nowCompiled(false),
	m_frame(0),
	m_fps(0),
	m_baseTime(-1),
	//sound_system("Anuch-Metro.mp3"),
	m_scenes("settings.xml") {

	m_scenes.setEffectLoadCallback(SET_CALLBACK_1(Core::loadEffectCallback, this));
}

int Core::init(const Size& windows) {
	m_windowSize = windows;
	//Sound
	//sound_system.play_music();

#if 0
	audio_spectr_l = new float[SPECTRUMSIZE];
	audio_spectr_r = new float[SPECTRUMSIZE];
#endif

	//glew
	if (glewInit() != GLEW_OK) {
		LOG("Error: glewInit()");
		return -1;
	}
	//Textures
	m_scenes.init(true);

	//BitmapFontGL::Instance()->CreateTexture();
	//glGenTextures(1, &audioTexture);
	//baseTime = -1;
	m_nowSource.clear();
	return 0;
}

void Core::free() {
	m_scenes.free();

	if (NULL != m_audioTexture)
		glDeleteTextures(1, &m_audioTexture);
	m_audioTexture = NULL;
	//delete audioAnalyzer;
	SAFE_DELETE(m_audioSpectrL);
	SAFE_DELETE(m_audioSpectrR);
}

int Core::changeKeys(const Uint8 *state, bool press) {
	if (m_scenes.changeKeys(state, press))
		return 1;
	return 0;
}

void Core::loadEffectCallback(uint effectId) {
	//LOGF("curr Effect = %i", effectId);
	//textEditor.Load(EffectFileTable[effectId]);
}

int Core::update(Uint32 nowTime) {
	if (-1 == m_baseTime) m_baseTime = nowTime;
	float realSec = ( nowTime - m_baseTime ) / 1000.0f;
	float delay = ( nowTime - m_baseTime ) / 1000.0f;
	m_frame++;
	if (delay >= .5f) {
		m_prevTime = nowTime;
		m_fps = (int) ( (float) m_frame / delay );
		m_frame = 0;
	}
	//prevTime = nowTime;
	m_scenes.update();

	//sound_system.update();
	//sound_system.get_spectrum(audio_spectr_l, audio_spectr_r);

	render(realSec);
	return 0;
}

void Core::render(float realSec) {
	m_scenes.draw(realSec);
	return;
	//scenes.swapRenderTarget();
	//scenes.clear();

	/*
	std::string str = textEditor.ToString();
	if (nowSource != str)
	{
		nowSource = str;

		if (shaderGL[nowEffect].Compile(nowSource) != 0)
			nowCompiled = true;
		else
			nowCompiled = false;
	}
	*/
	float low = 0.0f;
	float mid = 0.0f;
	float high = 0.0f;
	float cy = -9999.0f;

	const int fontWidth = 10;
	const int fontHeight = 15;
	//BitmapFontGL::Instance()->SetFontSize(fontWidth, fontHeight);

	if (m_scenes.isValid()) {
		//scenes.begin(realSec);
#if 0 //todo
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
					for (int y = 0; y < image_height; y++) {
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
#endif
		//#####
		if (false) {
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
		//scenes.end();
	}

#if 0
	if (editMode) {
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

		if (textEditor.GetLineOffset() == 0) {
			upAlpha = false;
		}

		if (textEditor.GetLineOffset() + textEditor.GetMaxLineNum() >= textEditor.GetLineNum()) {
			downAlpha = false;
		}

		const int transRange = 5;
		BitmapFontGL::Instance()->Reset();
		for (int i = 0; i < textEditor.GetLineOffset(); i++) {
			BitmapFontGL::Instance()->ProcessComment(*textbuf[i]);
		}

		std::set<int> errorLinesFS = shaderGL[nowEffect].GetErrorLinesFS();

		for (int i = 0; i < ptrbuf.size(); i++) {
			float up = 1.0f;
			float down = 1.0f;

			if (upAlpha) {
				if (i < transRange) {
					up = (float) i / transRange;
					down = (float) ( i + 1 ) / transRange;
				}
			}
			if (downAlpha) {
				if (i >= ptrbuf.size() - transRange) {
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

		if (textEditor.IsSelectMode()) {
			EditorCursor selectStart = textEditor.GetSelectStart();
			if (selectStart.col > cursor.col)
				std::swap(selectStart, cursor);

			if (selectStart.col == cursor.col) {
				int start = selectStart.row;
				int end = cursor.row;


				BitmapFontGL::Instance()->DrawSelect(aspect, width, selectStart.col, start, end, 0.5f, 0.5f, 0.5f);
			}
			else {
				for (int i = selectStart.col; i <= cursor.col; i++) {
					if (i < 0 || i >= textEditor.GetMaxLineNum())
						continue;

					if (i == selectStart.col) {
						BitmapFontGL::Instance()->DrawSelect(aspect, width, i, selectStart.row, textEditor.GetLineLength(i));
					}
					else if (i < cursor.col) {
						BitmapFontGL::Instance()->DrawSelect(aspect, width, i, 0, textEditor.GetLineLength(i));
					}
					else if (i == cursor.col) {
						BitmapFontGL::Instance()->DrawSelect(aspect, width, i, 0, cursor.row);
					}
				}
			}
		}
		//		BitmapFontGL::Instance()->DrawSelect(aspect, width, 5, 1, 9);

		glPopMatrix();
	}
#endif


	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

#if ENABLE_POST_FX
	if (shaderGL[POSTFxID].Valid()) {
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
#endif
	{
		//scenes.draw();
	}
}
