{-# LANGUAGE DataKinds                  #-}
{-# LANGUAGE DeriveAnyClass             #-}
{-# LANGUAGE OverloadedStrings          #-}
{-# LANGUAGE TypeOperators              #-}
{-# LANGUAGE DeriveGeneric              #-}

module Web.Botan.Sdk
    ( track
    , shortenUrl
    ) where

import           Control.Monad.Trans.Except (ExceptT, runExceptT)
import           Data.Proxy
import           Data.Text (Text)
import           Network.HTTP.Client (Manager)
import           Servant.API
import           Servant.Client
import qualified Data.Aeson as A
import           GHC.Generics

botanBaseUrl :: BaseUrl
botanBaseUrl = BaseUrl Https "api.botan.io" 443 ""

data Result = Result
  {
    status :: Text
  , info :: Maybe Text
  } deriving (Show, Generic, A.FromJSON)

-- | Tracks Messages
track :: Text -> Text -> A.Value -> Text -> Manager -> IO (Either ServantError Result)
track token userId message eventName manager = runExceptT $ track_ (Just token) (Just userId) (Just eventName) message manager botanBaseUrl

shortenUrl :: Text -> Text -> Text -> Manager -> IO (Either ServantError Text)
shortenUrl token url userId manager = runExceptT $ shortenUrl_ (Just token) (Just url) (Just userId) manager botanBaseUrl

type BotanAPI =
  "track"
      :> QueryParam "token" Text
      :> QueryParam "uid" Text
      :> QueryParam "name" Text
      :> ReqBody '[JSON] A.Value
      :> Post '[JSON] Result
  :<|> "s/"
      :> QueryParam "token" Text
      :> QueryParam "url" Text
      :> QueryParam "user_ids" Text
      :> Get '[PlainText] Text

api :: Proxy BotanAPI
api = Proxy

track_ :: Maybe Text -> Maybe Text -> Maybe Text -> A.Value -> Manager -> BaseUrl -> ExceptT ServantError IO Result
shortenUrl_ :: Maybe Text -> Maybe Text -> Maybe Text -> Manager -> BaseUrl -> ExceptT ServantError IO Text
track_
  :<|> shortenUrl_ = client api