template<uint_fast64_t size>
	class BufferTexture {
			uint8_t cpuData[size] = {};
			GLuint bufHandle = 0;
			GLuint texHandle = 0;
		public:
			void init() {
				glGenBuffers(1, &bufHandle);
				glBindBuffer(GL_TEXTURE_BUFFER, bufHandle);
				glBufferData(GL_TEXTURE_BUFFER, size, cpuData, GL_DYNAMIC_DRAW);
				glGenTextures(1, &texHandle);
				glBindBuffer(GL_TEXTURE_BUFFER, 0);
			}
			~BufferTexture() {
				glDeleteTextures(1, &texHandle);
				glDeleteBuffers(1, &bufHandle);
			}
			template<typename T>
			[[nodiscard]] uint_fast64_t getCapacity() const {
				return size / sizeof(T);
			}
			template<typename T>
			bool writeToCpu(uint_fast64_t idx, uint_fast64_t num, T const *data) {
				if ( idx < 0 || num <= 0 || ! data) { return false;}
				if (idx * sizeof(T) + num * sizeof(T) >= size) { return false; }
				memcpy(&reinterpret_cast<T *>(cpuData)[idx], data, num * sizeof(T));
				return true;
			}
			template<typename T>
			bool writeToCpu(uint_fast64_t idx, const T &data) {
				return writeToCpu<T>(idx, 1, &data);
			}
			template<typename T>
			T *cpu(uint_fast64_t idx) {
				if (idx < 0 || idx * sizeof(T) + sizeof(T) > size) {
					return nullptr;
				}
				return &reinterpret_cast<T *>(cpuData)[idx];
			}
			void sendToGpu() {
				glBindBuffer(GL_TEXTURE_BUFFER, bufHandle);
				GLvoid *p = glMapBuffer(GL_TEXTURE_BUFFER, GL_WRITE_ONLY);
				memcpy(p, cpuData, size);
				glUnmapBuffer(GL_TEXTURE_BUFFER);
				glBindBuffer(GL_TEXTURE_BUFFER, 0);
			}
			void use(GLenum unit) {
				glActiveTexture(GL_TEXTURE0 + unit);
				glBindTexture(GL_TEXTURE_BUFFER, texHandle);
				glTexBuffer(GL_TEXTURE_BUFFER, GL_R32UI, bufHandle);
			}
	};
