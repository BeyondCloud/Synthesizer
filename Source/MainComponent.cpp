/*
==============================================================================

This file was auto-generated!

==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
class MainContentComponent : public AudioAppComponent
{
public:
	//==============================================================================
	MainContentComponent()
		: phase(0.0f),
		phaseDelta(0.0f),
		frequency(5000.0f),
		amplitude(0.2f),
		sampleRate(0.0),
		expectedSamplesPerBlock(0)
	{
		setSize(800, 600);

		// specify the number of input and output channels that we want to open
		setAudioChannels(0, 2);
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
		const float originalPhase = phase;

		for (int chan = 0; chan < bufferToFill.buffer->getNumChannels(); ++chan)
		{
			phase = originalPhase;

			float* const channelData = bufferToFill.buffer->getWritePointer(chan, bufferToFill.startSample);

			for (int i = 0; i < bufferToFill.numSamples; ++i) //numSamples = 480
			{
				channelData[i] = amplitude * std::sin(phase);

				// increment the phase step for the next sample
				phase = std::fmod(phase + phaseDelta, float_Pi * 2.0f);
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
		const float radius = amplitude * 200.0f;

		// Draw an ellipse based on the mouse position and audio volume
		g.setColour(Colours::lightgreen);
		g.fillEllipse(lastMousePosition.x - radius / 2.0f,
			lastMousePosition.y - radius / 2.0f,
			radius, radius);

		// draw a representative sinewave
		Path wavePath;
		wavePath.startNewSubPath(0, centreY);

		for (float x = 1.0f; x < getWidth(); ++x)
			wavePath.lineTo(x, centreY + amplitude * getHeight() * 2.0f
				* std::sin(x * frequency * 0.0001f));
		g.setColour(Colours::mediumpurple);
		g.strokePath(wavePath, PathStrokeType(2.0f));



		g.setColour(Colours::yellow);
		g.setFont(14.0f);
		g.drawText(juce::String(frequency), 20, 40, 200, 40, true);
		for (int i = 0; i <13; i++)
			g.drawText(toneName[i], toneFreq[i] / xScale - fixX, 40, 30, 40, true);

	}

	// Mouse handling..
	void mouseDown(const MouseEvent& e) override
	{
		mouseDrag(e);
	}

	void mouseDrag(const MouseEvent& e) override
	{
		lastMousePosition = e.position;

		    frequency = (e.x ) * xScale ;
		amplitude = jmin(0.2f, 0.2f - 0.2f * e.position.y / getHeight());

		phaseDelta = (float)(2.0 * double_Pi * frequency / sampleRate);

		repaint();
	}

	void mouseUp(const MouseEvent&) override
	{
		amplitude = 0.0f;
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
	float phase;
	float phaseDelta;
	float frequency;
	float amplitude;
	double sampleRate;
	float xScale = 0.5f; // x distance between two note,the smaller the wider
	int fixX = 3; // shift left 
	int expectedSamplesPerBlock;
	int toneFreq[13] = { 261,277,293,311,329,349,369,392,415,440,466,493,523 };
	String toneName[13] = { "C4","C4#","D4","D4#","E4","F4","F4#","G4","G#4","A4","A4#","B4","C5" };
	Point<float> lastMousePosition;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};


Component* createMainContentComponent() { return new MainContentComponent(); };

#endif  // MAINCOMPONENT_H_INCLUDED
