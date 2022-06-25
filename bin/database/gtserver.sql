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

 Date: 25/06/2022 16:16:40
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for players
-- ----------------------------
DROP TABLE IF EXISTS `players`;
CREATE TABLE `players`  (
  `user_id` int NOT NULL,
  `role` int NOT NULL,
  `tank_id_name` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `username` varchar(18) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `raw_name` varchar(18) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `raw_pass` varchar(24) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `email` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `discord` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `last_ip_address` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `base_ip_address` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `relative_identifier` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `machine_address` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `first_login` tinyint NOT NULL,
  `gems` int NOT NULL DEFAULT 0,
  `total_punched` bigint NOT NULL DEFAULT 0,
  `clothes` blob NOT NULL,
  `inventory` blob NOT NULL,
  `character_state` blob NOT NULL,
  `character_level` blob NOT NULL,
  `state_flags` blob NOT NULL,
  `playmods` blob NOT NULL,
  `trade_history` blob NOT NULL,
  `notebook` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `heart_monitor` blob NOT NULL,
  `guild_id` int NOT NULL DEFAULT -1,
  `banner_bandolier` blob NOT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of players
-- ----------------------------

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
