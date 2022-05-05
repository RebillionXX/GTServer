/*
 Navicat Premium Data Transfer

 Source Server         : local
 Source Server Type    : MySQL
 Source Server Version : 100424
 Source Host           : localhost:3306
 Source Schema         : gtserver

 Target Server Type    : MySQL
 Target Server Version : 100424
 File Encoding         : 65001

 Date: 05/05/2022 10:35:08
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for server_data
-- ----------------------------
DROP TABLE IF EXISTS `server_data`;
CREATE TABLE `server_data`  (
  `user_identifier` int NOT NULL,
  `online_players` int NOT NULL
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of server_data
-- ----------------------------
INSERT INTO `server_data` VALUES (0, 0);

SET FOREIGN_KEY_CHECKS = 1;
