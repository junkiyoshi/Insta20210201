#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");
	
	ofBackground(0);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	this->number_of_targets = 4 * 5 * 5;
	for (int i = 0; i < this->number_of_targets; i++) {

		this->target_list.push_back(glm::vec2());
		this->color_list.push_back(glm::vec3());
	}

	this->shader.load("shader/shader.vert", "shader/shader.frag");
}
//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	if (ofGetFrameNum() % 60 < 45) {

		this->noise_seed += ofMap(ofGetFrameNum() % 60, 0, 45, 0.04, 0);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	int range = 100;
	int radius = 20;
	for (int i = 0; i < this->number_of_targets; i += 4) {

		int x = (i / 4) % 5 * 144 + 72;
		int y = (i / 4) / 5 * 144 + 72;

		this->target_list[i] = glm::vec2(x + ofMap(ofNoise(ofRandom(1000), this->noise_seed), 0, 1, range * -0.5, range * 0.5), y + ofMap(ofNoise(ofRandom(1000), this->noise_seed), 0, 1, range * -0.5, range * 0.5));

		auto r_location = glm::vec2(x + ofMap(ofNoise(ofRandom(1000), this->noise_seed), 0, 1, range * -0.5, range * 0.5), y + ofMap(ofNoise(ofRandom(1000), this->noise_seed), 0, 1, range * -0.5, range * 0.5));
		auto g_location = glm::vec2(x + ofMap(ofNoise(ofRandom(1000), this->noise_seed), 0, 1, range * -0.5, range * 0.5), y + ofMap(ofNoise(ofRandom(1000), this->noise_seed), 0, 1, range * -0.5, range * 0.5));
		auto b_location = glm::vec2(x + ofMap(ofNoise(ofRandom(1000), this->noise_seed), 0, 1, range * -0.5, range * 0.5), y + ofMap(ofNoise(ofRandom(1000), this->noise_seed), 0, 1, range * -0.5, range * 0.5));

		auto r = ofMap(distance(this->target_list[i], r_location), 0, range, 1, 0.1);
		auto g = ofMap(distance(this->target_list[i], g_location), 0, range, 1, 0.1);
		auto b = ofMap(distance(this->target_list[i], b_location), 0, range, 1, 0.1);
		this->color_list[i] = glm::vec3(r, g, b);

		this->target_list[i + 1] = r_location;
		this->color_list[i + 1] = glm::vec3(0.3, 0.2, 0.2);
		this->target_list[i + 2] = g_location;
		this->color_list[i + 2] = glm::vec3(0.2, 0.3, 0.2);
		this->target_list[i + 3] = b_location;
		this->color_list[i + 3] = glm::vec3(0.2, 0.2, 0.3);

		ofSetColor(this->color_list[i].x * 255, this->color_list[i].y * 255, this->color_list[i].z * 255);

		auto r_angle = std::atan2(r_location.y - this->target_list[i].y, r_location.x - this->target_list[i].x);
		auto r_line_location = this->target_list[i] + glm::vec2(radius * cos(r_angle), radius * sin(r_angle));
		ofDrawLine(r_location, r_line_location);

		auto g_angle = std::atan2(g_location.y - this->target_list[i].y, g_location.x - this->target_list[i].x);
		auto g_line_location = this->target_list[i] + glm::vec2(radius * cos(g_angle), radius * sin(g_angle));
		ofDrawLine(g_location, g_line_location);

		auto b_angle = std::atan2(b_location.y - this->target_list[i].y, b_location.x - this->target_list[i].x);
		auto b_line_location = this->target_list[i] + glm::vec2(radius * cos(b_angle), radius * sin(b_angle));
		ofDrawLine(b_location, b_line_location);
	}

	ofFill();
	this->shader.begin();
	this->shader.setUniform1f("time", ofGetElapsedTimef());
	this->shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	this->shader.setUniform2fv("targets", &this->target_list[0].x, this->number_of_targets);
	this->shader.setUniform3fv("colors", &this->color_list[0].x, this->number_of_targets);

	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	this->shader.end();

	for (int i = 0; i < this->number_of_targets; i++) {

		if (i % 4 == 0) {

			ofSetColor(this->color_list[i].x * 255, this->color_list[i].y * 255, this->color_list[i].z * 255);

			ofBeginShape();
			for (int deg = 0; deg <= 360; deg += 1) {

				ofVertex(this->target_list[i] + glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD)));
			}
			for (int deg = 360; deg >= 0; deg -= 1) {

				ofVertex(this->target_list[i] + glm::vec2((radius + 1) * cos(deg * DEG_TO_RAD), (radius + 1) * sin(deg * DEG_TO_RAD)));
			}
			ofEndShape(true);
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(720, 720);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}