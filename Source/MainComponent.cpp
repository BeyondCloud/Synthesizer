/*
==============================================================================

This file was auto-generated!

==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"
#include "ShareMem.h"

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED
#define MAX_TOUCH 2
TCHAR szName[] = TEXT("bitmap");
struct blob {
	int x = 0;
	int y = 0;
	int size = 0;
};
ShareMem sh(szName);
blob* MemPtr = (blob *)sh.readMem();
blob blobGroup[10];

//==============================================================================
class MainContentComponent : public AudioAppComponent
							,public Timer
{
public:
	//==============================================================================
	MainContentComponent()
		:sampleRate(0.0),
		expectedSamplesPerBlock(0)
	{
		for (int i = 0; i < MAX_TOUCH; i++)
		{
			note[i].phase = 0;
			note[i].phaseDelta = 0;
			note[i].frequency = 5000;
			note[i].amplitude = 0.2f;
		}
		setSize(800, 600);

		// specify the number of input and output channels that we want to open
		setAudioChannels(0, 2);
	//	startTimer(1);
	}

	~MainContentComponent()
	{
		shutdownAudio();
	}

	//=======================================================================
	void prepareToPlay(int samplesPerBlockExpected, double newSampleRate) override
	{
		sampleRate = newSampleRate;
		expectedSamplesPerBlock = samplesPerBlockExpected;
	}

	/*  This method generates the actual audio samples.
	In this example the buffer is filled with a sine wave whose frequency and
	amplitude are controlled by the mouse position.
	*/
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
	{
		
		bufferToFill.clearActiveBufferRegion();
		for (int i = 0; i < MAX_TOUCH; i++)
			note[i].originalPhase = note[i].phase;
		
		for (int chan = 0; chan < bufferToFill.buffer->getNumChannels(); ++chan)
		{
			for (int i = 0; i < MAX_TOUCH; i++)
				note[i].phase = note[i].originalPhase;

			float* const channelData = bufferToFill.buffer->getWritePointer(chan, bufferToFill.startSample);

			for (int i = 0; i < bufferToFill.numSamples; ++i) //numSamples = 480
			{
				// increment the phase step for the next sample
				for (int k = 0; k < MAX_TOUCH; k++)
				{
					channelData[i] += note[k].amplitude * (std::sin(note[k].phase));
					note[k].phase = std::fmod(note[k].phase + note[k].phaseDelta, float_Pi * 2.0f);
				}
				
			}
		}
	}
	
	void releaseResources() override
	{
		// This gets automatically called when audio device parameters change
		// or device is restarted.
	}


	//=======================================================================
	void paint(Graphics& g) override
	{
		// (Our component is opaque, so we must completely fill the background with a solid colour)
		g.fillAll(Colours::black);

		const float centreY = getHeight() / 2.0f;
		const float radius = note[0].amplitude * 200.0f;

		// Draw an ellipse based on the mouse position and audio volume
		g.setColour(Colours::lightgreen);
		g.fillEllipse(lastMousePosition.x - radius / 2.0f,
			lastMousePosition.y - radius / 2.0f,
			radius, radius);

		// draw a representative sinewave
		Path wavePath;
		wavePath.startNewSubPath(0, centreY);

		for (float x = 1.0f; x < getWidth(); ++x)
			wavePath.lineTo(x, centreY + note[0].amplitude * getHeight() * 2.0f
				* std::sin(x * note[0].frequency * 0.0001f));
		g.setColour(Colours::mediumpurple);
		g.strokePath(wavePath, PathStrokeType(2.0f));
		g.setColour(Colours::yellow);
		g.setFont(14.0f);
		g.drawText(juce::String(note[0].amplitude), 20, 40, 200, 40, true);

		g.drawText(juce::String(blobGroup[0].size), 80, 80, 200, 40, true);
		
		for (int i = 0; i < 13; i++)
			g.drawText(toneName[i], toneFreq[i] / xScale - fixX, 40, 30, 40, true);

	}

	// Mouse handling..
	void mouseDown(const MouseEvent& e) override
	{
		mouseDrag(e);
	}
	void timerCallback() override 
	{
		updatePosition();
	}

	
	void mouseDrag(const MouseEvent& e) override
	{
		lastMousePosition = e.position;
		note[0].frequency = (e.x) * xScale;



		note[0].amplitude = jmin(0.2f, 0.2f - 0.2f * e.position.y / getHeight());

		for (int i = 0; i < MAX_TOUCH; i++)
		{
			note[i].phaseDelta = (float)(2.0 * double_Pi * note[i].frequency / sampleRate);
		}
		for (int i = 1; i < MAX_TOUCH; i++)
		{
			note[i].amplitude = 0;
		}
		
		repaint();
	}
	
	void updatePosition() 
	{
		if (MemPtr != NULL)
		{
			lastMousePosition = Point<float>(blobGroup[0].x, blobGroup[0].y);

			for (int i = 0; i < MAX_TOUCH; i++)
			{
				blobGroup[i] = *(MemPtr + i);
				if (blobGroup[i].size != 0)
				{
					note[i].frequency = (blobGroup[i].x) * xScale + 100;
					float temp = 0.2f - 0.2f * blobGroup[i].y / getHeight();
					note[i].amplitude = 0.2f > temp ? temp : 0.2f;
					for (int i = 0; i < MAX_TOUCH; i++)
						note[i].phaseDelta = (float)(2.0 * double_Pi * note[i].frequency / sampleRate);
					repaint();
				}
				else
				{
					note[i].amplitude = 0.0f;
					repaint();
				}
			}
		}
	}
	void mouseUp(const MouseEvent&) override
	{
		note[0].amplitude = 0.0f;
		repaint();
	}

	void resized() override
	{
		// This is called when the MainContentComponent is resized.
		// If you add any child components, this is where you should
		// update their positions.
	}


private:
	//==============================================================================
	
	double sampleRate;
	float xScale = 1.0f; // x distance between two note,the smaller the wider
	int fixX = 3; // shift left 
	int expectedSamplesPerBlock;
	int toneFreq[13] = { 261,277,293,311,329,349,369,392,415,440,466,493,523 };
	String toneName[13] = { "C4","C4#","D4","D4#","E4","F4","F4#","G4","G#4","A4","A4#","B4","C5" };
	Point<float> lastMousePosition;
	struct note_t 
	{
		float phase;
		float phaseDelta;
		float frequency;
		float originalPhase;
		float amplitude;
	};
	note_t note[MAX_TOUCH];

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};


Component* createMainContentComponent() { return new MainContentComponent(); };

#endif  // MAINCOMPONENT_H_INCLUDED
